#ifndef FLUIDSIM_RENDER_SYSTEM_H
#define FLUIDSIM_RENDER_SYSTEM_H

#include "Kikan/ecs/systems/IRenderSystem.h"
#include "Grid.h"
#include "Kikan/ecs/Scene.h"
#include "Controls.h"

class RenderSystem : public Kikan::IRenderSystem {
public:
    explicit RenderSystem(Controls* controls, Kikan::Scene* scene);

    void update(double dt) override;

    void setGrid(Grid* grid);
private:
    Grid* _grid{};

    Controls* _controls;
    Kikan::Scene* _scene;

    void gen_quads();
};

#endif //FLUIDSIM_RENDER_SYSTEM_H
