#include <iostream>
#include "FluidSim/SimulationSystem.h"
#include "Kikan/util/Timer.h"

SimulationSystem::SimulationSystem(DistanceField* distanceField, Constants* constants, Controls* controls, Stats* stats, Kikan::Scene* scene, GridRenderSystem* rs)
    : _distanceField(distanceField), _constants(constants), _controls(controls), _stats(stats), _scene(scene), _rs(rs){
    singleInclude(Particle);

    _grid = new Grid(glm::vec2(0,0), _distanceField->getWidth() / _constants->RADIUS, _distanceField->getHeight() / _constants->RADIUS, _constants->RADIUS);
    _rs->setGrid(_grid);
}

SimulationSystem::~SimulationSystem() {
    delete _grid;
}

void SimulationSystem::update(double dt) {
    dt = 33.3333;
    auto dtf = (float) dt;

    apply_controls(dtf);

    if(_controls->PAUSE || _controls->LOADING)
        return;

    apply_external_forces(dtf);
    apply_viscosity(dtf);
    advance_particles(dtf);
    update_neighbours();
    double_density_relaxation(dtf);
    resolve_collisions(dtf);
    update_velocity(dtf);
    update_stats();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-flp30-c"
void SimulationSystem::apply_controls(float dt) {
    _extern_force = false;

    if(_input->mousePressed(Kikan::Mouse::BUTTON_1) &&
        !_controls->LOADING &&
        !_controls->POPUP_OPEN &&
        _controls->MOUSE_IN_SPACE &&
        _controls->MOUSE_X > 0 && _controls->MOUSE_X < (float)_distanceField->getWidth() &&
        _controls->MOUSE_Y > 0 && _controls->MOUSE_Y < (float)_distanceField->getHeight())
    {
        switch (_controls->BRUSH_MODE) {
            case Controls::BMT::M_SPAWN: {

                if(!_controls->SINGLE_SPAWN || (_controls->SINGLE_SPAWN && !_controls->SINGLE_SPAWN_SPAWNED)){
                    float penSizeH = _controls->PEN_SIZE / 2.f;
                    for (float x = -penSizeH; x <= penSizeH; x += (float)_constants->RADIUS) {
                        for (float y = -penSizeH; y <= penSizeH; y += (float)_constants->RADIUS) {
                            if(glm::length(glm::vec2(x, y)) < penSizeH){
                                glm::vec2 pos = glm::vec2(_controls->MOUSE_X + x, _controls->MOUSE_Y + y);
                                auto* entity = new Kikan::Entity();
                                auto* particle = new Particle();
                                particle->pos = pos;
                                particle->ppos = pos;
                                particle->vel = glm::vec2(0,0);
                                entity->addComponent(particle);
                                _scene->addEntity(entity);
                            }
                        }
                    }

                    update_neighbours();
                }
                _controls->SINGLE_SPAWN_SPAWNED = true;

                break;
            }
            case Controls::BMT::M_ERASE: {

                for (int i = 0; i < _entities.size(); ++i) {
                    auto* p = _entities[i]->getComponent<Particle>();
                    if(glm::length(p->pos - glm::vec2(_controls->MOUSE_X, _controls->MOUSE_Y)) < _controls->ERASER_SIZE / 2.f) {
                        _scene->deleteEntity(_entities[i]);
                        i--;
                    }
                }
                break;
            }
            case Controls::BMT::M_FORCE: {
                _extern_force = true;
                break;
            }
        }
    }
    else{
        _controls->SINGLE_SPAWN_SPAWNED = false;
    }

    if(_controls->REBUILD){
        delete _grid;
        _grid = new Grid(glm::vec2(0, 0), _distanceField->getWidth() / _constants->RADIUS, _distanceField->getHeight() / _constants->RADIUS, _constants->RADIUS);
        _rs->setGrid(_grid);
        _controls->REBUILD = false;
        _controls->RESET = true;
    }

    if(_controls->RESET){
        _controls->RESET = false;
        _lost_ps = 0;
        for (int i = 0; i < _entities.size(); ++i) {
            _scene->deleteEntity(_entities[i]);
            i--;
        }
    }
}
#pragma clang diagnostic pop


void SimulationSystem::apply_external_forces(float dt) {
    Kikan::Timer timer(&_stats->PERFORMANCE["Apply External Forces"], Kikan::Timer::Precision::MICRO);

    for (Kikan::Entity* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        p->vel += glm::vec2(0, -.05) * dt / 1000.f; // Gravity

        if(_extern_force){
            glm::vec2 force = (p->pos - glm::vec2(_controls->MOUSE_X, _controls->MOUSE_Y));
            if(force.x != 0 && force.y != 0){
                force = glm::normalize(force) * _controls->FORCE * (_controls->FORCE_MODE == Controls::FMT::M_PUSH ? 1.f : -1.f);
                p->vel += force * dt / 1000.f;
            }
        }
    }
}

void SimulationSystem::apply_viscosity(float dt) {
    Kikan::Timer timer(&_stats->PERFORMANCE["Apply Viscosity"], Kikan::Timer::Precision::MICRO);

    for (auto* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        for (auto* n : _p_neighbours[p]) {
            if(p->pos == n->pos)
                n->pos += glm::vec2 (1);

            glm::vec2 q = (p->pos - n->pos) / _constants->RADIUS;
            if(glm::length(q) < 1.f){
                glm::vec2 u = (p->vel - n->vel) * glm::normalize((p->pos - n->pos));
                if(glm::length(u) > 0){
                    glm::vec2 I = dt * (1.f - q) * (_constants->SIGMA * u + _constants->BETA * u * u) * glm::normalize((p->pos - n->pos));
                    p->vel -= .5f * I;
                    n->vel += .5f * I;
                }
            }
        }
    }
}

void SimulationSystem::advance_particles(float dt) {
    Kikan::Timer timer(&_stats->PERFORMANCE["Advance Particles"], Kikan::Timer::Precision::MICRO);

    _grid->clear();
    for (auto* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        p->ppos = p->pos;
        p->pos += dt * p->vel;

        limit_pos(p);

        _grid->moveParticle(p);
    }
}

void SimulationSystem::update_neighbours() {
    Kikan::Timer timer(&_stats->PERFORMANCE["Update Neighbors"], Kikan::Timer::Precision::MICRO);

    for (auto* entity : _entities) {
        auto* p = entity->getComponent<Particle>();

        _p_neighbours[p].clear();
        _grid->possibleNeighbours(_p_neighbours[p], p);
    }
}

void SimulationSystem::double_density_relaxation(float dt) {
    Kikan::Timer timer(&_stats->PERFORMANCE["Double Density Relaxation"], Kikan::Timer::Precision::MICRO);

    for (auto* entity : _entities) {
        auto *i = entity->getComponent<Particle>();

        glm::vec2 p = glm::vec2(0);
        glm::vec2 p_near = glm::vec2(0);

        for (Particle* j : _p_neighbours[i]) {
            if(i->pos == j->pos)
                j->pos += glm::vec2 (1);

            glm::vec2 q = (i->pos - j->pos) / _constants->RADIUS;
            if(glm::length(q) < 1.f){
                p += (1.f - q) * (1.f - q);
                p_near += (1.f - q) * (1.f - q) * (1.f - q);
            }
        }

        glm::vec2 P = _constants->STIFFNESS * (p - _constants->P0);
        glm::vec2 P_near = _constants->STIFFNESS_NEAR * p_near;

        glm::vec2 dx = glm::vec2(0);
        for (Particle* j : _p_neighbours[i]) {
            if(i->pos == j->pos)
                j->pos += glm::vec2 (0.01);

            glm::vec2 q = (i->pos - j->pos) / _constants->RADIUS;
            if(glm::length(q) < 1.f){
                glm::vec2 D = dt * dt * (P * (1.f - q) + P_near * (1.f - q) * (1.f - q)) * glm::normalize(i->pos - j->pos);
                j->pos += 0.5f * D;
                dx -= 0.5f * D;
            }
        }
        i->pos += dx;
        limit_pos(i);
    }
}

void SimulationSystem::resolve_collisions(float dt) {
    Kikan::Timer timer(&_stats->PERFORMANCE["Resolve Collision"], Kikan::Timer::Precision::MICRO);

    for (auto* entity : _entities) {
        auto *p = entity->getComponent<Particle>();

        //p->pos.x = std::max(p->pos.x, 0.f);
        //p->pos.y = std::max(p->pos.y, 0.f);
        //p->pos.x = std::min(p->pos.x, 200.f);
        //p->pos.y = std::min(p->pos.y, 200.f);

        int dist = _distanceField->distance(p->pos);
        if(dist > -_constants->COLLISION_RADIUS){
            glm::vec2 normal(0);
            _distanceField->normal(p->pos, normal);
            glm::vec2 tangent(-normal.y, normal.x);
            tangent *= dt * _constants->FRICTION * glm::dot(glm::normalize(p->pos - p->ppos), tangent);
            p->pos -= tangent;
            p->pos -= .8f * (float)(dist + _constants->COLLISION_RADIUS) * normal;

            limit_pos(p);
        }
    }
}

void SimulationSystem::update_velocity(float dt) {
    Kikan::Timer timer(&_stats->PERFORMANCE["Update Velocity"], Kikan::Timer::Precision::MICRO);

    for (auto* entity : _entities) {
        auto *p = entity->getComponent<Particle>();
        p->vel = (p->pos - p->ppos) / dt;
    }
}

void SimulationSystem::setDistanceField(DistanceField* distanceField) {
    _distanceField = distanceField;
}

void SimulationSystem::update_stats() {
    Kikan::Timer timer(&_stats->PERFORMANCE["Update Stats"], Kikan::Timer::Precision::MICRO);

    for (int i = 0; i < _entities.size(); ++i) {
        auto* p = _entities[i]->getComponent<Particle>();
        if(std::isnan(p->pos.x) || std::isnan(p->pos.y)){
            _lost_ps++;
            _scene->deleteEntity(_entities[i]);
            i--;
        }
    }

    _stats->LOST_PARTICLES = _lost_ps;
    _stats->PARTICLES = _entities.size();
}

Grid *SimulationSystem::getGrid() {
    return _grid;
}

void SimulationSystem::limit_pos(Particle* p){
    p->pos.x = std::min(std::max(p->pos.x, 2 * _constants->RADIUS), (float)_distanceField->getWidth() - 2 * _constants->RADIUS);
    p->pos.y = std::min(std::max(p->pos.y, 2 * _constants->RADIUS), (float)_distanceField->getHeight() - 2 * _constants->RADIUS);
}