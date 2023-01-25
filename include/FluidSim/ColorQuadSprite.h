#ifndef FLUIDSIM_COLOR_QUAD_SPRITE_H
#define FLUIDSIM_COLOR_QUAD_SPRITE_H

#include "glm/glm.hpp"
#include "Kikan/ecs/components/IComponent.h"

struct ColorQuadSprite : Kikan::IComponent {
public:
    glm::vec2 points[4];
    glm::vec4 color[4];
    float layer;
};


#endif //FLUIDSIM_COLOR_QUAD_SPRITE_H
