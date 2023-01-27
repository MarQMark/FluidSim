#ifndef FLUIDSIM_PARTICLE_RENDER_SYSTEM_H
#define FLUIDSIM_PARTICLE_RENDER_SYSTEM_H

#include "Kikan/ecs/systems/IRenderSystem.h"
#include "Stats.h"
#include "Controls.h"
#include "Constants.h"

class ParticleRenderSystem : public Kikan::IRenderSystem {
public:
    ParticleRenderSystem(Controls* controls, Constants* constants, Stats* stats);

    void update(double dt) override;

private:

    Controls* _controls;
    Constants* _constants;
    Stats* _stats;

    std::vector<Kikan::DefaultVertex> _vertices;
    std::vector<GLuint> _indices;

    void update_vertices();
};

#endif //FLUIDSIM_PARTICLE_RENDER_SYSTEM_H
