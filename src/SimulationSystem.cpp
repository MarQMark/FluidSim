#include <iostream>
#include <chrono>
#include "FluidSim/SimulationSystem.h"
#include "FluidSim/Particle.h"
#include "Kikan/ecs/components/Texture2DSprite.h"

#define RADIUS 5.0f
#define SIGMA 0.00000005f
#define BETA 0.000000005f

SimulationSystem::SimulationSystem(DistanceField* distanceField) {
    singleInclude(Particle);

    _distanceField = distanceField;
    _grid = new Grid(glm::vec2(0,0), 20, 20, 10);
}

SimulationSystem::~SimulationSystem() {
    delete _distanceField;
    delete _grid;
}

int i = 0;
void SimulationSystem::update(double dt) {
    //if(!_input->keyPressed(Kikan::Key::R)){
    //    i = 0;
    //    return;
    //}
    //if(i != 0)
    //    return;
    //i++;

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

    resolve_collisions();
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
}

void SimulationSystem::apply_external_forces(double dt) {
    for (Kikan::Entity* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        p->vel += glm::vec2(0, -.005); // Gravity
        // Add mouse input or other forces
    }
}

void SimulationSystem::apply_viscosity(double dt) {
    for (auto* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        for (auto* n : _p_neighbours[p]) {

            glm::vec2 v_pn = n->pos - p->pos;
            glm::vec2 vel_i = (p->vel - n->vel) * v_pn;

            if(vel_i.x > 0 && vel_i.y > 0){
                float length = std::max(glm::length(v_pn), 0.001f);
                vel_i /= length;
                v_pn /= length;
                float q = length / RADIUS;
                float k = .5f * (float)dt * (1.f - q);
                glm::vec2 I = k * (SIGMA * vel_i + BETA * vel_i * vel_i) * v_pn;
                p->vel -= I;
            }
        }
    }
}

void SimulationSystem::advance_particles(double dt) {
    for (auto* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        p->ppos = p->pos;
        p->pos += (float)dt * p->vel;

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
            if(glm::length(p->pos - possibleP->pos) < RADIUS && p != possibleP)
                _p_neighbours[p].push_back(possibleP);
        }
    }
}

#define STIFFNESS 0.00002f
#define STIFFNESS_NEAR 0.0002f
void SimulationSystem::double_density_relaxation(double dt) {
    for (auto* entity : _entities) {
        auto *p = entity->getComponent<Particle>();

        double press = 0;
        double press_near = 0;

        for (Particle* n : _p_neighbours[p]) {
            float q = 1.f - std::max(glm::length(p->pos - n->pos), 1e-10f) / RADIUS;
            press += q * q;
            press_near += q * q * q;
        }

        press = std::max(press, -1e5);
        press = std::min(press, 1e5);
        press_near = std::max(press_near, -1e5);
        press_near = std::min(press_near, 1e5);

        float P = STIFFNESS * (press - 50.f);//p0);
        float P_near = STIFFNESS_NEAR * press_near;

        glm::vec2 D = glm::vec2(0);
        glm::vec2 delta = glm::vec2(0);
        for (Particle* n : _p_neighbours[p]) {
            float q = 1.f - std::max(glm::length(p->pos - n->pos), 1e-10f) / RADIUS;
            glm::vec2 v = (p->pos - n->pos) / std::max(glm::length(p->pos - n->pos), 0.001f);
            D = (float)(.5f * dt * dt * (P * q + P_near * q * q)) * v;

            D.x = std::min(D.x, 100.f);
            D.x = std::max(D.x, -100.f);
            D.y = std::min(D.y, 100.f);
            D.y = std::max(D.y, -100.f);

            n->pos = n->pos + D;

            delta = delta - D;
        }
        p->pos = p->pos + D;
    }
}

void SimulationSystem::resolve_collisions() {
    for (auto* entity : _entities) {
        auto *p = entity->getComponent<Particle>();

        p->pos.x = std::max(p->pos.x, 0.f);
        p->pos.y = std::max(p->pos.y, 0.f);
        p->pos.x = std::min(p->pos.x, 200.f);
        p->pos.y = std::min(p->pos.y, 200.f);
    }
}

void SimulationSystem::update_velocity(double dt) {
    for (auto* entity : _entities) {
        auto *p = entity->getComponent<Particle>();
        p->vel = (p->pos - p->ppos) / (float)dt;
    }
}

void SimulationSystem::update_sprite() {
    for (auto* entity : _entities) {
        auto *p = entity->getComponent<Particle>();

        auto* sprite = entity->getComponent<Kikan::Texture2DSprite>();
        glm::vec2 delta = p->pos - sprite->points[0] + glm::vec2(-5, 5);
        sprite->points[0] += delta;
        sprite->points[1] += delta;
        sprite->points[2] += delta;
        sprite->points[3] += delta;
    }
}
