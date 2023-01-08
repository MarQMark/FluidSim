#ifndef KIKAN_PHYSICS_H
#define KIKAN_PHYSICS_H

#include "glm/glm.hpp"
#include "IComponent.h"

namespace Kikan {
    struct Physics : IComponent {
        glm::vec2 _velocity;
        glm::vec2 _acceleration;
    };
}

#endif //KIKAN_PHYSICS_H
