#ifndef FLUIDSIM_PARTICLE_H
#define FLUIDSIM_PARTICLE_H

#include "glm/glm.hpp"
#include "Kikan/ecs/components/IComponent.h"

struct Particle : Kikan::IComponent{
public:
    glm::vec2 pos;
    glm::vec2 ppos;
    glm::vec2 vel;
    float index;
};

#endif //FLUIDSIM_PARTICLE_H
