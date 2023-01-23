#include <iostream>
#include <chrono>
#include "FluidSim/SimulationSystem.h"
#include "FluidSim/Particle.h"
#include "Kikan/ecs/components/Texture2DSprite.h"

SimulationSystem::SimulationSystem(DistanceField* distanceField, Constants* constants, Controls* controls, Stats* stats, Kikan::Scene* scene)
    : _distanceField(distanceField), _constants(constants), _controls(controls), _stats(stats), _scene(scene){
    singleInclude(Particle);

    _grid = new Grid(glm::vec2(0,0), _distanceField->getWidth() / _constants->RADIUS, _distanceField->getHeight() / _constants->RADIUS, _constants->RADIUS);
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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-flp30-c"
void SimulationSystem::update(double dt) {

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
        _controls->REBUILD = false;
        _controls->RESET = true;
    }

    if(_controls->RESET){
        _controls->RESET = false;
        for (int i = 0; i < _entities.size(); ++i) {
            _scene->removeEntity(_entities[i]);
            i--;
        }
    }

    if(_controls->PAUSE || _controls->LOADING)
        return;

    dt = 33.3333;

    auto time = std::chrono::high_resolution_clock::now();

    apply_external_forces(dt);
    if(_input->keyPressed(Kikan::Key::P))
        std::cout << ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - time)).count() << "   apply_external_forces" << std::endl;
    time = std::chrono::high_resolution_clock::now();

    apply_viscosity(dt);
    if(_input->keyPressed(Kikan::Key::P))
        std::cout << ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - time)).count() << "   apply_viscosity" << std::endl;
    time = std::chrono::high_resolution_clock::now();

    advance_particles(dt);
    if(_input->keyPressed(Kikan::Key::P))
        std::cout << ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - time)).count() << "   advance_particles" << std::endl;
    time = std::chrono::high_resolution_clock::now();

    update_neighbours();
    if(_input->keyPressed(Kikan::Key::P))
        std::cout << ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - time)).count() << "   update_neighbours" << std::endl;
    time = std::chrono::high_resolution_clock::now();

    double_density_relaxation(dt);
    if(_input->keyPressed(Kikan::Key::P))
        std::cout << ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - time)).count() << "   double_density_relaxation" << std::endl;
    time = std::chrono::high_resolution_clock::now();

    resolve_collisions(dt);
    if(_input->keyPressed(Kikan::Key::P))
        std::cout << ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - time)).count() << "   resolve_collisions" << std::endl;
    time = std::chrono::high_resolution_clock::now();

    update_velocity(dt);
    if(_input->keyPressed(Kikan::Key::P))
        std::cout << ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - time)).count() << "   update_velocity" << std::endl;
    time = std::chrono::high_resolution_clock::now();

    update_sprite();
    if(_input->keyPressed(Kikan::Key::P))
        std::cout << ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - time)).count() << "   update_sprite" << std::endl;
    time = std::chrono::high_resolution_clock::now();

    update_stats();
    if(_input->keyPressed(Kikan::Key::P))
        std::cout << ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - time)).count() << "   update_stats" << std::endl;
}
#pragma clang diagnostic pop

void SimulationSystem::apply_external_forces(float dt) {
    for (Kikan::Entity* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        p->vel += glm::vec2(0, -.005); // Gravity

        if(_extern_force){
            glm::vec2 force = (p->pos - glm::vec2(_controls->MOUSE_X, _controls->MOUSE_Y));
            if(force.x != 0 && force.y != 0){
                force = glm::normalize(force) * _controls->FORCE * (_controls->FORCE_MODE == Controls::FMT::M_PUSH ? 1.f : -1.f);
                p->vel += force;
            }
        }
    }
}

/*void SimulationSystem::apply_viscosity(float dt) {
    for (auto* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        for (auto* n : _p_neighbours[p]) {

            glm::vec2 v_pn = n->pos - p->pos;
            glm::vec2 vel_i = (p->vel - n->vel) * v_pn;

            if(vel_i.x > 0 && vel_i.y > 0){
                if(p->pos == n->pos)
                    n->pos += glm::vec2 (0.01);

                float length = glm::length(v_pn);
                vel_i /= length;
                v_pn /= length;
                float q = length / _constants->RADIUS;
                float k = .5f * dt * (1.f - q);
                glm::vec2 I = k * (_constants->SIGMA * vel_i + _constants->BETA * vel_i * vel_i) * v_pn;
                p->vel -= I;
            }
        }
    }
}*/

void SimulationSystem::apply_viscosity(float dt) {
    for (auto* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        for (auto* n : _p_neighbours[p]) {
            if(p->pos == n->pos)
                n->pos += glm::vec2 (0.01);

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
    for (auto* entity : _entities) {
        auto* p = entity->getComponent<Particle>();

        //TODO: Maybe init vectors
        _p_neighbours[p].clear();
        std::vector<Particle*> possibleN;
        _grid->possibleNeighbours(possibleN, p);
        for (Particle* possibleP : possibleN) {
            if(p->pos != possibleP->pos && glm::length(p->pos - possibleP->pos) < _constants->RADIUS)
                _p_neighbours[p].push_back(possibleP);
        }
    }
}

/*void SimulationSystem::double_density_relaxation(float dt) {
    for (auto* entity : _entities) {
        auto *p = entity->getComponent<Particle>();

        double press = 0;
        double press_near = 0;

        for (Particle* n : _p_neighbours[p]) {
            if(p->pos == n->pos)
                n->pos += glm::vec2 (0.01);

            float q = 1.f - glm::length(p->pos - n->pos) / _constants->RADIUS;
            press += q * q;
            press_near += q * q * q;
        }

        press = std::max(press, -_constants->MAX_PRESS);
        press = std::min(press, _constants->MAX_PRESS);
        press_near = std::max(press_near, -_constants->MAX_PRESS);
        press_near = std::min(press_near, _constants->MAX_PRESS);

        float P = (float)(_constants->STIFFNESS * (press - _constants->P0));
        float P_near = (float)(_constants->STIFFNESS_NEAR * press_near);

        glm::vec2 D = glm::vec2(0);
        glm::vec2 delta = glm::vec2(0);
        for (Particle* n : _p_neighbours[p]) {
            if(p->pos == n->pos)
                n->pos += glm::vec2 (0.01);

            float q = 1.f - glm::length(p->pos - n->pos) / _constants->RADIUS;
            glm::vec2 v = (p->pos - n->pos) / glm::length(p->pos - n->pos);
            D = .5f * dt * dt * (P * q + P_near * q * q) * v;

            D.x = std::min(D.x, _constants->MAX_D);
            D.x = std::max(D.x, -_constants->MAX_D);
            D.y = std::min(D.y, _constants->MAX_D);
            D.y = std::max(D.y, -_constants->MAX_D);

            n->pos = n->pos + D;

            delta = delta - D;
        }
        p->pos = p->pos + delta;
    }
}*/

void SimulationSystem::double_density_relaxation(float dt) {
    for (auto* entity : _entities) {
        auto *i = entity->getComponent<Particle>();

        glm::vec2 p = glm::vec2(0);
        glm::vec2 p_near = glm::vec2(0);

        for (Particle* j : _p_neighbours[i]) {
            if(i->pos == j->pos)
                j->pos += glm::vec2 (0.01);

            glm::vec2 q = (i->pos - j->pos) / _constants->RADIUS;
            if(glm::length(q) < 1.f){
                p += (1.f - q) * (1.f - q);
                p_near += (1.f - q) * (1.f - q) * (1.f - q);
            }
        }

        glm::vec2 P = _constants->STIFFNESS * (p - _constants->P0);
        glm::vec2 P_near = _constants->STIFFNESS_NEAR * p_near;

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

