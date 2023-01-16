#include <iostream>
#include "Kikan/Engine.h"
#include "Kikan/ecs/components/Texture2DSprite.h"
#include "Kikan/ecs/components/QuadSprite.h"
#include "Kikan/ecs/systems/SpriteRenderSystem.h"
#include "Kikan/opengl/buffers/Texture2D.h"
#include "FluidSim/Particle.h"
#include "FluidSim/SimulationSystem.h"
#include "stb_image/stb_image.h"

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

#define TEXTURE_SIZE 500
#define TEXTURE_SIZE_HALF (TEXTURE_SIZE / 2.f)
int WinMain() {
    Kikan::Engine engine;

#pragma region Shader
    engine.getRenderer()->shader()->changeFs(Kikan::Shader::loadShaderSource("shaders/main.frag"));

    //glClearColor(0.2, 0.3, 0.7, 1.);
    glClearColor(0., 0., 0., 0.);

    //load max texture units in sampler in default frag shader
    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    GLint textures[maxTextureUnits];
    for(int i = 0; i < maxTextureUnits; i++) textures[i] = i;
        engine.getRenderer()->shader()->uniform1iv("u_texture", maxTextureUnits, textures);

    glBlendEquation( GL_FUNC_ADD);

#pragma endregion

    std::string fileName("assets/tube");

#pragma region background
    //create Box background
    int mapWidth;
    int mapHeight;
    int mapBPP;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* buff = stbi_load((fileName + ".png").c_str(), &mapWidth, &mapHeight, &mapBPP, 4);
    Kikan::Texture2D boxTxt(mapWidth, mapHeight, buff);
#pragma endregion

    std::string datafile(fileName + ".dat");
    auto* df = new DistanceField(glm::vec2(-50, -50), mapWidth, mapHeight, buff, datafile);

    engine.getScene()->addSystem(new Kikan::SpriteRenderSystem());
    engine.getScene()->addSystem(new SimulationSystem(df));

    engine.getScene()->addEntity(createBox(glm::vec2(-50, 250), 300, 300, boxTxt.get()));

#pragma region particles
    //create Texture
    std::vector<float> data(TEXTURE_SIZE * TEXTURE_SIZE * 4);
    for (int x = 0; x < TEXTURE_SIZE; ++x) {
        for (int y = 0; y < TEXTURE_SIZE; ++y) {
            data[(x + TEXTURE_SIZE * y) * 4] = 0.;
            data[(x + TEXTURE_SIZE * y) * 4 + 1] = 0.;
            data[(x + TEXTURE_SIZE * y) * 4 + 2] = 0.;
            data[(x + TEXTURE_SIZE * y) * 4 + 3] = (float)(1.f - std::sqrt((x - TEXTURE_SIZE_HALF) * (x - TEXTURE_SIZE_HALF) + (y - TEXTURE_SIZE_HALF) * (y - TEXTURE_SIZE_HALF)) / TEXTURE_SIZE_HALF);
        }
    }
    Kikan::Texture2D texture2D(TEXTURE_SIZE, TEXTURE_SIZE, data.data());

    //create Fluid
    for (int i = 0; i < 1000; ++i) {
        engine.getScene()->addEntity(createParticle(glm::vec2(rand() % 50, rand() % 50 + 150), 10, 10, texture2D.get()));
    }
#pragma endregion

    engine.getScene()->camera()->scale(1 / 150.f * (720. / 1280.), 1 / 150.0f);
    engine.getScene()->camera()->translate(-100, -100);

    //glfwSwapInterval(1);

    while (engine.shouldRun()){
        engine.update();
    }

    return 0;
}

#ifdef __linux__
int main(){
    WinMain();
}
#endif