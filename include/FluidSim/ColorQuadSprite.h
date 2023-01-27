#ifndef FLUIDSIM_COLOR_QUAD_SPRITE_H
#define FLUIDSIM_COLOR_QUAD_SPRITE_H

#include "Kikan/ecs/components/IComponent.h"

struct ColorQuadSprite : Kikan::IComponent {
public:
    Kikan::DefaultVertex v;
};


#endif //FLUIDSIM_COLOR_QUAD_SPRITE_H
