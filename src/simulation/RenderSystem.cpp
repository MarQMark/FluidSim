#include "FluidSim/RenderSystem.h"
#include "FluidSim/ColorQuadSprite.h"
#include "Kikan/util/Timer.h"


RenderSystem::RenderSystem(Controls* controls, Kikan::Scene* scene) : _scene(scene), _controls(controls){
    singleInclude(ColorQuadSprite);
}

void RenderSystem::update(double dt) {
    if(_controls->RENDER_MODE != Controls::RMT::GRID)
        return;

    if(_input->keyPressed(Kikan::Key::P))
        Kikan::Timer timer("Render");

    std::vector<Kikan::IVertex *> vertices(_entities.size() * 4);
    std::vector<GLuint> indices(_entities.size() * 6);

    for (int i = 0; i < _entities.size(); ++i) {
        auto* s = _entities[i]->getComponent<ColorQuadSprite>();

        // Update Color
        glm::vec4 color = glm::vec4(0, 0, 1 - (float)_grid->cellCount(i) / 5.f, _grid->cellCount(i) != 0 ? 1 : 0);

        float cells[9];
        cells[0] = (float)_grid->cellCount(i + _grid->getWidth() - 1) / 5.f;
        cells[1] = (float)_grid->cellCount(i + _grid->getWidth()) / 5.f;
        cells[2] = (float)_grid->cellCount(i + _grid->getWidth() + 1) / 5.f;
        cells[3] = (float)_grid->cellCount(i - 1) / 5.f;
        cells[4] = (float)_grid->cellCount(i) / 5.f;
        cells[5] = (float)_grid->cellCount(i + 1) / 5.f;
        cells[6] = (float)_grid->cellCount(i - _grid->getWidth() - 1) / 5.f;
        cells[7] = (float)_grid->cellCount(i - _grid->getWidth()) / 5.f;
        cells[8] = (float)_grid->cellCount(i - _grid->getWidth() + 1) / 5.f;

        float tl = (cells[0] + cells[1] + cells[3] + cells[4]) / 4.f;
        float tr = (cells[1] + cells[2] + cells[4] + cells[5]) / 4.f;
        float bl = (cells[3] + cells[4] + cells[6] + cells[7]) / 4.f;
        float br = (cells[4] + cells[5] + cells[7] + cells[8]) / 4.f;

        float alpha[9];
        alpha[0] = (float)_grid->cellCount(i + _grid->getWidth() - 1) != 0 ? 1 : 0;
        alpha[1] = (float)_grid->cellCount(i + _grid->getWidth()) != 0 ? 1 : 0;
        alpha[2] = (float)_grid->cellCount(i + _grid->getWidth() + 1) != 0 ? 1 : 0;
        alpha[3] = (float)_grid->cellCount(i - 1) != 0 ? 1 : 0;
        alpha[4] = (float)_grid->cellCount(i) != 0 ? 1 : 0;
        alpha[5] = (float)_grid->cellCount(i + 1) != 0 ? 1 : 0;
        alpha[6] = (float)_grid->cellCount(i - _grid->getWidth() - 1) != 0 ? 1 : 0;
        alpha[7] = (float)_grid->cellCount(i - _grid->getWidth()) != 0 ? 1 : 0;
        alpha[8] = (float)_grid->cellCount(i - _grid->getWidth() + 1) != 0 ? 1 : 0;

        float atl = (alpha[0] + alpha[1] + alpha[3] + alpha[4]) / 2.f;
        float atr = (alpha[1] + alpha[2] + alpha[4] + alpha[5]) / 2.f;
        float abl = (alpha[3] + alpha[4] + alpha[6] + alpha[7]) / 2.f;
        float abr = (alpha[4] + alpha[5] + alpha[7] + alpha[8]) / 2.f;

        s->color[0] = glm::vec4(0, 0, 1 - tl, atl);
        s->color[1] = glm::vec4(0, 0, 1 - tr, atr);
        s->color[2] = glm::vec4(0, 0, 1 - br, abr);
        s->color[3] = glm::vec4(0, 0, 1 - bl, abl);

        // Push to Renderer

        auto* v1 = new Kikan::DefaultVertex;
        v1->position = glm::vec3(s->points[0].x, s->points[0].y, s->layer);
        v1->textureCoords = glm::vec2(0.0);
        v1->color = s->color[0];
        v1->texture = -1;
        vertices[4 * i + 0] = v1;

        auto* v2 = new Kikan::DefaultVertex;
        v2->position = glm::vec3(s->points[1].x, s->points[1].y, s->layer);
        v2->textureCoords = glm::vec2(0.0);
        v2->color = s->color[1];
        v2->texture = -1;
        vertices[4 * i + 1] = v2;

        auto* v3 = new Kikan::DefaultVertex;
        v3->position = glm::vec3(s->points[2].x, s->points[2].y, s->layer);
        v3->textureCoords = glm::vec2(0.0);
        v3->color = s->color[2];
        v3->texture = -1;
        vertices[4 * i + 2] = v3;

        auto* v4 = new Kikan::DefaultVertex;
        v4->position = glm::vec3(s->points[3].x, s->points[3].y, s->layer);
        v4->textureCoords = glm::vec2(0.0);
        v4->color = s->color[3];
        v4->texture = -1;
        vertices[4 * i + 3] = v4;

        indices[6 * i + 0] = 0 + 4 * i;
        indices[6 * i + 1] = 1 + 4 * i;
        indices[6 * i + 2] = 2 + 4 * i;
        indices[6 * i + 3] = 0 + 4 * i;
        indices[6 * i + 4] = 2 + 4 * i;
        indices[6 * i + 5] = 3 + 4 * i;
    }

    _renderer->autoBatch<Kikan::DefaultVertex>(vertices, &indices);

    for (auto* v : vertices) {
        delete v;
    }
}

void RenderSystem::setGrid(Grid* grid) {
    _grid = grid;

    for (auto* e : _entities) {
        delete e;
    }
    _entities.clear();

    gen_quads();
}

void RenderSystem::gen_quads() {
    for (int y = 0; y < _grid->getHeight(); ++y) {
        for (int x = 0; x < _grid->getWidth(); ++x) {
            auto* e = new Kikan::Entity();
            auto* s = new ColorQuadSprite();

            glm::vec2 d = glm::vec2(x, y) * _grid->getSize();

            s->points[0] = glm::vec2(0,1) * _grid->getSize() + d;
            s->points[1] = glm::vec2(1,1) * _grid->getSize() + d;
            s->points[2] = glm::vec2(1,0) * _grid->getSize() + d;
            s->points[3] = glm::vec2(0,0) * _grid->getSize() + d;

            s->color[0] = glm::vec4(0.8, .4, .2, 1.);
            s->color[1] = glm::vec4(0.8, .4, .2, 1.);
            s->color[2] = glm::vec4(0.8, .4, .2, 1.);
            s->color[3] = glm::vec4(0.8, .4, .2, 1.);

            s->layer = -1.;

            e->addComponent(s);
            _entities.push_back(e);
        }
    }
}
