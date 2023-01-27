#ifndef FLUIDSIM_FLUID_SIMULATION_H
#define FLUIDSIM_FLUID_SIMULATION_H

#include "Kikan/Engine.h"
#include "Kikan/opengl/buffers/Texture2D.h"
#include "DistanceField.h"
#include "ViewSpace.h"
#include "ConstantsEditor.h"
#include "FluidSim/MapFile.h"
#include "MapTree.h"
#include "SimulationSystem.h"
#include "../../src/file_browser/ImGuiFileBrowser.h"
#include "StatsViewer.h"
#include "GridRenderSystem.h"
#include "ParticleRenderSystem.h"

class FluidSimulation : Kikan::Renderer::Override {
public:
    explicit FluidSimulation();
    ~FluidSimulation();

    void update() const;
    bool shouldRun() const;

private:
    void preRender(Kikan::Renderer* renderer, double dt) override;
    void postRender(Kikan::Renderer* renderer, double dt) override;

    Kikan::Engine* _engine;
    Kikan::Texture2D* _particle2D;
    SimulationSystem* _sim_system;
    GridRenderSystem* _g_render_system;
    ParticleRenderSystem* _p_render_system;
    Kikan::Entity* _background;

    ConstantsEditor* _ce;
    StatsViewer* _sv;

    Kikan::Texture2D* _view_space_2D;
    float _view_space_ratio = 1;
    GLuint _fbo{};
    ViewSpace* _vs;

    imgui_addons::ImGuiFileBrowser _fileBrowser;
    MapTree* _mt;
    std::vector<MapFile*> _maps;
    MapFile* _curr_map;

    float _loading = 1;
    std::string _loading_msg;

    void render_dockspace();
    void render_ui();
};


#endif //FLUIDSIM_FLUID_SIMULATION_H
