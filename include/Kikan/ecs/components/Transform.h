#ifndef KIKAN_TRANSFORM_H
#define KIKAN_TRANSFORM_H

#include "glm/glm.hpp"
#include "IComponent.h"

namespace Kikan {
    struct Transform : IComponent {
        glm::vec3 _position = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 _scale = glm::vec3(1.0, 1.0, 1.0);
        glm::vec3 _rotation = glm::vec3(1.0, 1.0, 1.0);
    };
}

#endif //KIKAN_TRANSFORM_H
