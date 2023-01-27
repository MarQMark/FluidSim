#include <iostream>
#include "FluidSim/SimulationSystem.h"
#include "Kikan/ecs/components/Texture2DSprite.h"
#include "Kikan/util/Timer.h"

SimulationSystem::SimulationSystem(DistanceField* distanceField, Constants* constants, Controls* controls, Stats* stats, Kikan::Scene* scene, RenderSystem* rs)
    : _distanceField(distanceField), _constants(constants), _controls(controls), _stats(stats), _scene(scene), _rs(rs){
    singleInclude(Particle);

    _grid = new Grid(glm::vec2(0,0), _distanceField->getWidth() / _constants->RADIUS, _distanceField->getHeight() / _constants->RADIUS, _constants->RADIUS);
    _rs->setGrid(_grid);
}

SimulationSystem::~SimulationSystem() {
    delete _grid;
}

Kikan::Entity* createParticle2(glm::vec2 pos, float w, float h, GLuint txtID){
    auto* entity = new Kikan::Entity();

    auto* sprite = new Kikan::Texture2DSprite();
    sprite->points[0] = pos + glm::vec2(-w / 2.f, h / 2.f);
    sprite->points[1] = pos + glm::vec2(w / 2.f, h / 2.f);
    sprite->points[2] = pos + glm::vec2(w / 2.f, -h / 2.f);
    sprite->points[3] = pos + glm::vec2(-w / 2.f, -h / 2.f);
    sprite->color = glm::vec4(0, 0, .8, 1.);
    sprite->textureID = txtID;
    sprite->layer = -1;
    entity->addComponent(sprite);

    auto* particle = new Particle();
    particle->pos = pos;
    particle->ppos = pos;
    particle->vel = glm::vec2(0,0);
    particle->index = 0;
    entity->addComponent(particle);

    return entity;
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
    update_sprite();
    update_stats();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-flp30-c"
void SimulationSystem::apply_controls(float dt) {
    _extern_force = false;

    if(_input->mousePressed(Kikan::Mouse::BUTTON_1) &&
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
                            if(glm::length(glm::vec2(x, y)) < penSizeH)
                                _scene->addEntity(createParticle2(glm::vec2(_controls->MOUSE_X + x, _controls->MOUSE_Y + y), 10, 10, _constants->TEXTURE_ID));
                        }
                    }
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
            _scene->removeEntity(_entities[i]);
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

        p->pos.x = std::max(p->pos.x, _constants->RADIUS);
        p->pos.y = std::max(p->pos.y, _constants->RADIUS);
        p->pos.x = std::min(p->pos.x, (float)_distanceField->getWidth() - _constants->RADIUS);
        p->pos.y = std::min(p->pos.y, (float)_distanceField->getHeight() - _constants->RADIUS);

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

        P.x = std::max(P.x, -_constants->MAX_PRESS);
        P.x = std::min(P.x, _constants->MAX_PRESS);
        P.y = std::max(P.y, -_constants->MAX_PRESS);
        P.y = std::min(P.y, _constants->MAX_PRESS);
        P_near.x = std::max(P_near.x, -_constants->MAX_PRESS);
        P_near.x = std::min(P_near.x, _constants->MAX_PRESS);
        P_near.y = std::max(P_near.y, -_constants->MAX_PRESS);
        P_near.y = std::min(P_near.y, _constants->MAX_PRESS);

        i->index = P.y;

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

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void SimulationSystem::update_sprite() {
    if(_controls->RENDER_MODE != Controls::RMT::PARTICLES)
        return;

    Kikan::Timer timer(&_stats->PERFORMANCE["Update Sprite"], Kikan::Timer::Precision::MICRO);

    float max = -10000;
    float min = 10000;
    for (auto* entity : _entities) {
        auto *p = entity->getComponent<Particle>();

        max = std::max(max, p->index);
        min = std::min(min, p->index);
    }

    for (auto* entity : _entities) {
        auto *p = entity->getComponent<Particle>();

        auto* sprite = entity->getComponent<Kikan::Texture2DSprite>();
        glm::vec2 delta = p->pos - sprite->points[0] + glm::vec2(-5, 5);
        sprite->points[0] += delta;
        sprite->points[1] += delta;
        sprite->points[2] += delta;
        sprite->points[3] += delta;
        sprite->color = glm::vec4(.0f , .0f, map(p->index, min, max, 1, 0), 1.f);
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
            _scene->removeEntity(_entities[i]);
            i--;
        }
    }

    _stats->LOST_PARTICLES = _lost_ps;
    _stats->PARTICLES = _entities.size();
}

Grid *SimulationSystem::getGrid() {
    return _grid;
}
