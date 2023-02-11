#ifndef KIKAN_QUAD_SPRITE_H
#define KIKAN_QUAD_SPRITE_H

#include "glm/glm.hpp"
#include "IComponent.h"

namespace Kikan {
    struct QuadSprite : IComponent {
        glm::vec2 points[4];
        glm::vec4 color;
        float layer;

        void destroy() override{
            delete this;
        };
    };
}

#endif //KIKAN_QUAD_SPRITE_H
