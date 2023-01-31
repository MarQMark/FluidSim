#ifndef FLUIDSIM_RENDER_SYSTEM_H
#define FLUIDSIM_RENDER_SYSTEM_H

#include "Kikan/ecs/systems/IRenderSystem.h"
#include "Grid.h"
#include "Kikan/ecs/Scene.h"
#include "Controls.h"
#include "Stats.h"
#include "Kikan/opengl/buffers/Texture2D.h"

class GridRenderSystem : public Kikan::IRenderSystem {
public:
    explicit GridRenderSystem(Controls* controls, Stats* stats);
    ~GridRenderSystem() override;

    void update(double dt) override;

    void setGrid(Grid* grid);
private:
    Grid* _grid{};
    uint64_t _cellCount = 0;

    Kikan::Texture2D* _texture;

    Stats* _stats;
    Controls* _controls;

    std::vector<Kikan::DefaultVertex> _vertices;
    std::vector<GLuint> _indices;
};

#endif //FLUIDSIM_RENDER_SYSTEM_H
