#ifndef KIKAN_TRIANGLE_SPRITE_H
#define KIKAN_TRIANGLE_SPRITE_H

#include "glm/glm.hpp"
#include "IComponent.h"

namespace Kikan {
    struct TriangleSprite : IComponent {
        glm::vec2 points[3];
        glm::vec4 color;
        float layer;
    };
}

#endif //KIKAN_TRIANGLE_SPRITE_H
