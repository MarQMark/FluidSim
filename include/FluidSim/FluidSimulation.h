#ifndef FLUIDSIM_FLUID_SIMULATION_H
#define FLUIDSIM_FLUID_SIMULATION_H

#include "Kikan/Engine.h"
#include "Kikan/opengl/buffers/Texture2D.h"
#include "DistanceField.h"
#include "ViewSpace.h"
#include "ConstantsEditor.h"


class FluidSimulation : Kikan::Renderer::Override {
public:
    explicit FluidSimulation(std::string& map);
    ~FluidSimulation();

    void update() const;
    bool shouldRun() const;

private:
    void preRender() override;
    void postRender() override;

    Kikan::Engine* _engine;
    Kikan::Texture2D* _particle2D;
    Kikan::Texture2D* _map2D;

    ConstantsEditor* _ce;

    DistanceField* _df;

    Kikan::Texture2D* _view_space_2D;
    GLuint _fbo{};
    ViewSpace* _vs;

    void render_dockspace();
    void render_ui();
};


#endif //FLUIDSIM_FLUID_SIMULATION_H
