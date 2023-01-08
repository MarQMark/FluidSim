#include "Kikan/Engine.h"
#include "Kikan/ecs/components/Texture2DSprite.h"
#include "Kikan/ecs/components/QuadSprite.h"
#include "Kikan/ecs/systems/SpriteRenderSystem.h"
#include "Kikan/opengl/buffers/Texture2D.h"
#include "FluidSim/Particle.h"
#include "FluidSim/SimulationSystem.h"

Kikan::Entity* createParticle(glm::vec2 pos, float w, float h, GLuint txtID){
    auto* entity = new Kikan::Entity();

    auto* sprite = new Kikan::Texture2DSprite();
    sprite->points[0] = pos;
    sprite->points[1] = pos + glm::vec2(w, 0);
    sprite->points[2] = pos + glm::vec2(w, -h);
    sprite->points[3] = pos + glm::vec2(0, -h);
    sprite->color = glm::vec4(0, 0, .8, 1.);
    sprite->textureID = txtID;
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

int WinMain() {
    Kikan::Engine engine;
    engine.getRenderer()->shader()->changeFs(Kikan::Shader::loadShaderSource("shaders/main.frag"));

    engine.getScene()->addSystem(new Kikan::SpriteRenderSystem());
    engine.getScene()->addSystem(new SimulationSystem());

    //create Texture
    std::vector<float> data(500 * 500 * 4);
    for (int x = 0; x < 500; ++x) {
        for (int y = 0; y < 500; ++y) {
            data[(x + 500 * y) * 4] = 0.;
            data[(x + 500 * y) * 4 + 1] = 0.;
            data[(x + 500 * y) * 4 + 2] = 0.;
            data[(x + 500 * y) * 4 + 3] = (float)(1.f - std::sqrt((x - 250) * (x - 250) + (y - 250) * (y - 250)) / 250.0);
        }
    }
    Kikan::Texture2D texture2D(500, 500, data.data());

    //create Fluid
    engine.getScene()->addEntity(createParticle(glm::vec2(25, 150), 10, 10, texture2D.get()));
    for(int n = 0; n < 1; n++)
        for(int i = 0; i < 10; i++)
            engine.getScene()->addEntity(createParticle(glm::vec2(i * 10, 50 + n), 10, 10, texture2D.get()));

    //draw grid
    engine.getScene()->addEntity(createLine(glm::vec2(-400, -1), 800, 2));
    engine.getScene()->addEntity(createLine(glm::vec2(-1, 400), 2, 800));


    engine.getScene()->camera()->scale(1 / 400.0f, 1 / 400.0f * (1280. /720. ));

    glfwSwapInterval(1);

    while (engine.shouldRun()){
        engine.update();
    }

    return 0;
}