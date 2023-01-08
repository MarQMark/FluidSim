#include <iostream>
#include "FluidSim/SimulationSystem.h"
#include "FluidSim/Particle.h"
#include "Kikan/ecs/components/Texture2DSprite.h"

#define RADIUS 10.0f
#define SIGMA 0.1f
#define BETA 0.f

SimulationSystem::SimulationSystem() {
    singleInclude(Particle);

    _grid = new Grid(glm::vec2(0,0), 20, 20, 10);
}

SimulationSystem::~SimulationSystem() {
    delete _grid;
}

void SimulationSystem::update(double dt) {
    apply_external_forces(dt);
    apply_viscosity(dt);
    advance_particles(dt);
    update_neighbours();
    double_density_relaxation(dt);
    resolve_collisions();
    update_velocity(dt);
    update_sprite();
}

void SimulationSystem::apply_external_forces(double dt) {
    for (Kikan::Entity* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        p->vel += glm::vec2(0, -.0010); // Gravity
        // Add mouse input or other forces
    }
}

void SimulationSystem::apply_viscosity(double dt) {
    for (auto* entity : _entities) {
        auto* p = entity->getComponent<Particle>();
        for (auto* entity2 : _entities) {
            auto* n = entity2->getComponent<Particle>();

            glm::vec2 v_pn = n->pos - p->pos;
            glm::vec2 vel_i = (p->vel - n->vel) * v_pn;

            if(vel_i.x > 0 && vel_i.y > 0){
                float length = glm::length(v_pn);
                vel_i /= length;
                v_pn /= length;
                float q = length / RADIUS;
                float k = .5f * dt * (1.f - q);
                glm::vec2 I = k * (SIGMA * vel_i + BETA * vel_i * vel_i) * v_pn;
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

#define STIFFNESS 0.000002f
#define STIFFNESS_NEAR 0.000002f
void SimulationSystem::double_density_relaxation(double dt) {
    for (auto* entity : _entities) {
        auto *p = entity->getComponent<Particle>();

        float press = 0;
        float press_near = 0;

        for (Particle* n : _p_neighbours[p]) {
            float q = 1.f - std::max(glm::length(p->pos - n->pos), 0.001f) / RADIUS;
            press += q * q;
            press_near += q * q * q;
        }
        float P = STIFFNESS * (press - 50.f);//p0);
        float P_near = STIFFNESS_NEAR * press_near;

        glm::vec2 D;
        glm::vec2 delta = glm::vec2(0);
        for (Particle* n : _p_neighbours[p]) {
            float q = 1.f - std::max(glm::length(p->pos - n->pos), 0.001f) / RADIUS;
            glm::vec2 v = (p->pos - n->pos) / std::max(glm::length(p->pos - n->pos), 0.001f);
            D = (float)(.5f * dt * dt * (P * q + P_near * q * q)) * v;
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
        glm::vec2 delta = p->pos - sprite->points[0] + glm::vec2(5, 5);
        sprite->points[0] += delta;
        sprite->points[1] += delta;
        sprite->points[2] += delta;
        sprite->points[3] += delta;
    }
}
