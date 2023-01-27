#ifndef FLUIDSIM_SPRITE_RENDER_SYSTEM_H
#define FLUIDSIM_SPRITE_RENDER_SYSTEM_H

#include "Kikan/ecs/systems/IRenderSystem.h"
#include "Controls.h"
#include "Stats.h"

class SpriteRenderSystem : public Kikan::IRenderSystem {
public:
    explicit SpriteRenderSystem(Controls* controls, Stats* stats);

    void update(double dt) override;

private:
    Controls* _controls;
    Stats* _stats;
};


#endif //FLUIDSIM_SPRITE_RENDER_SYSTEM_H
