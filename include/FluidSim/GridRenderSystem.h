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
    explicit GridRenderSystem(Controls* controls, Stats* stats, Kikan::Scene* scene);
    ~GridRenderSystem() override;

    void update(double dt) override;

    void setGrid(Grid* grid);
private:
    Grid* _grid{};
    uint64_t _cellCount;

    Kikan::Texture2D* _texture;

    Stats* _stats;
    Controls* _controls;
    Kikan::Scene* _scene;

    std::vector<float> _rps; // Relative particle count grid
    std::vector<float> _alphas;

    std::vector<Kikan::DefaultVertex> _vertices;
    std::vector<GLuint> _indices;

    void update_color();
    void update_vertices();
    void gen_render_data();
};

#endif //FLUIDSIM_RENDER_SYSTEM_H
