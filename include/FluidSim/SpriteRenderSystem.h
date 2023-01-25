#ifndef FLUIDSIM_SPRITE_RENDER_SYSTEM_H
#define FLUIDSIM_SPRITE_RENDER_SYSTEM_H

#include "Kikan/ecs/systems/IRenderSystem.h"
#include "Controls.h"

class SpriteRenderSystem : public Kikan::IRenderSystem {
public:
    explicit SpriteRenderSystem(Controls* controls);

    void update(double dt) override;

private:
    Controls* _controls;
};


#endif //FLUIDSIM_SPRITE_RENDER_SYSTEM_H
