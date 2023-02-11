#ifndef KIKAN_POLYGON_SPRITE_H
#define KIKAN_POLYGON_SPRITE_H

#include <vector>
#include "glm/glm.hpp"
#include "IComponent.h"

namespace Kikan {
    struct PolygonSprite : IComponent {
        std::vector<glm::vec2> points;
        glm::vec4 color;
        float layer;

        void destroy() override{
            delete this;
        };
    };
}

#endif //KIKAN_POLYGON_SPRITE_H
