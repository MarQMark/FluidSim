#include "FluidSim/FluidSimulation.h"
#include <iostream>

#include "Kikan/ecs/components/QuadSprite.h"
#include "Kikan/ecs/systems/SpriteRenderSystem.h"
#include "Kikan/ecs/components/Texture2DSprite.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#define TEXTURE_SIZE 500
#define TEXTURE_SIZE_HALF (TEXTURE_SIZE / 2.f)

#define RESOLUTION 2048

Kikan::Entity* createParticle(glm::vec2 pos, float w, float h, GLuint txtID){
    auto* entity = new Kikan::Entity();

    auto* sprite = new Kikan::Texture2DSprite();
    sprite->points[0] = pos + glm::vec2(-w / 2.f, h / 2.f);
    sprite->points[1] = pos + glm::vec2(w / 2.f, h / 2.f);
    sprite->points[2] = pos + glm::vec2(w / 2.f, -h / 2.f);
    sprite->points[3] = pos + glm::vec2(-w / 2.f, -h / 2.f);
    sprite->color = glm::vec4(0, 0, .8, 1.);
    sprite->textureID = txtID;
    sprite->layer = -1;
    entity->addComponent(sprite);

    auto* particle = new Particle();
    particle->pos = pos;
    particle->ppos = pos;
    particle->vel = glm::vec2(0,0);
    particle->index = 0;
    entity->addComponent(particle);

    return entity;
}

Kikan::Entity* createLine(glm::vec2 pos, float w, float h){
    auto* entity = new Kikan::Entity();
    auto* sprite = new Kikan::QuadSprite();
    sprite->points[0] = pos;
    sprite->points[1] = pos + glm::vec2(w, 0);
    sprite->points[2] = pos + glm::vec2(w, -h);
    sprite->points[3] = pos + glm::vec2(0, -h);
    sprite->color = glm::vec4(1, 1, 1, 1.);
    entity->addComponent(sprite);
    return entity;
}

Kikan::Entity* createBox(glm::vec2 pos, float w, float h, GLuint txtID){
    auto* box = new Kikan::Entity();
    auto* sprite = new Kikan::Texture2DSprite();
    sprite->points[0] = pos;
    sprite->points[1] = pos + glm::vec2(w, 0);
    sprite->points[2] = pos + glm::vec2(w, -h);
    sprite->points[3] = pos + glm::vec2(0, -h);
    sprite->textureID = txtID;
    sprite->layer = -.5;
    box->addComponent(sprite);
    return box;
}

FluidSimulation::FluidSimulation() {

    //Setup Engine
    _engine = new Kikan::Engine();
    _engine->getRenderer()->shader()->changeFs(Kikan::Shader::loadShaderSource("shaders/main.frag"));
    _engine->getRenderer()->overrideRender(this);
    _engine->getRenderer()->shader()->uniform1li("u_sampler", 0);


    // Setup ImGUI
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_engine->getRenderer()->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430");


    // Setup Widgets
    glGenFramebuffers(1, &_fbo);
    _view_space_2D = new Kikan::Texture2D(RESOLUTION, RESOLUTION, (float*)nullptr);
    _vs = new ViewSpace(_view_space_2D);
    _ce = new ConstantsEditor();
    _mt = new MapTree(&_maps);


    // Load maps
    _maps.push_back(new MapFile("assets/box"));
    _maps.push_back(new MapFile("assets/tube"));
    _maps.push_back(new MapFile("assets/slope"));
    _curr_map = _maps[0];
    _engine->getRenderer()->shader()->uniform1lf("u_pTexture", (float)_maps.size() + 2);


    // Engine Stuff
    _engine->getScene()->addSystem(new Kikan::SpriteRenderSystem());
    _sim_system = new SimulationSystem(_curr_map->getDistanceField(), _ce->getConstants());
    _engine->getScene()->addSystem(_sim_system);

    _background = createBox(glm::vec2(0, (float)_curr_map->getHeight()), (float)_curr_map->getWidth(), (float)_curr_map->getHeight(), _curr_map->getTexture()->get());
    _engine->getScene()->addEntity(_background);


    //create Texture
    std::vector<float> data(TEXTURE_SIZE * TEXTURE_SIZE * 4);
    for (int x = 0; x < TEXTURE_SIZE; ++x) {
        for (int y = 0; y < TEXTURE_SIZE; ++y) {
            data[(x + TEXTURE_SIZE * y) * 4] = 0.;
            data[(x + TEXTURE_SIZE * y) * 4 + 1] = 0.;
            data[(x + TEXTURE_SIZE * y) * 4 + 2] = 1.;
            data[(x + TEXTURE_SIZE * y) * 4 + 3] = (float)(1.f - std::sqrt((x - TEXTURE_SIZE_HALF) * (x - TEXTURE_SIZE_HALF) + (y - TEXTURE_SIZE_HALF) * (y - TEXTURE_SIZE_HALF)) / TEXTURE_SIZE_HALF);
        }
    }
    _particle2D = new Kikan::Texture2D(TEXTURE_SIZE, TEXTURE_SIZE, data.data());

    //spawn Particles
    for (int i = 0; i < 1000; ++i) {
        _engine->getScene()->addEntity(createParticle(glm::vec2(rand() % 50 + 50, rand() % 50 + _curr_map->getHeight() - 100), 10, 10, _particle2D->get()));
    }
}

FluidSimulation::~FluidSimulation() {
    delete _engine;
    delete _particle2D;
    delete _view_space_2D;
    delete _ce;
    delete _mt;
    for (auto* map : _maps) {
        delete map;
    }
    glDeleteFramebuffers(1, &_fbo);
}

void FluidSimulation::update() const {
    _engine->update();
}

bool FluidSimulation::shouldRun() const {
    return _engine->shouldRun();
}

void FluidSimulation::preRender(Kikan::Renderer* renderer, double dt) {
    float ratio = ((float)_vs->getWidth() / (float)_vs->getHeight());
    _engine->getScene()->camera()->reset();
    if(_curr_map->getHeight() > _curr_map->getWidth())
        _engine->getScene()->camera()->scale(1 / ((float)_curr_map->getHeight() * _vs->getZoom() / 2.f * ratio), 1 / ((float)_curr_map->getHeight() * _vs->getZoom() / 2.f ));
    else
        _engine->getScene()->camera()->scale(1 / ((float)_curr_map->getWidth() * _vs->getZoom() / 2.f * ratio), 1 / ((float)_curr_map->getWidth() * _vs->getZoom() / 2.f ));
    _engine->getScene()->camera()->translate(-(float)_curr_map->getWidth() / 2, -(float)_curr_map->getHeight() / 2);

    if(_mt->getLoaded() != nullptr && _curr_map != _mt->getLoaded()){
        //TODO: DELETE OLD PARTICLES

        _curr_map = _mt->getLoaded();
        _ce->getConstants()->REBUILD = true;
        _sim_system->setDistanceField(_curr_map->getDistanceField());

        auto* sprite = _background->getComponent<Kikan::Texture2DSprite>();
        sprite->points[0] = glm::vec2(0, _curr_map->getHeight());
        sprite->points[1] = sprite->points[0] + glm::vec2(_curr_map->getWidth(), 0);
        sprite->points[2] = sprite->points[0] + glm::vec2(_curr_map->getWidth(), -_curr_map->getHeight());
        sprite->points[3] = sprite->points[0] + glm::vec2(0, -_curr_map->getHeight());
        sprite->textureID = _curr_map->getTexture()->get();
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, RESOLUTION, RESOLUTION);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _view_space_2D->get(), 0);
}

void FluidSimulation::postRender(Kikan::Renderer* renderer, double dt) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    render_dockspace();

    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void FluidSimulation::render_dockspace() {
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyBetterDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    // render own ui
    render_ui();

    ImGui::End();
}

void FluidSimulation::render_ui() {
    _vs->render();
    _ce->render();
    _mt->render();
}
