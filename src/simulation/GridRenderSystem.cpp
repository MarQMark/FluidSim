#include "FluidSim/GridRenderSystem.h"
#include "FluidSim/ColorQuadSprite.h"
#include "Kikan/util/Timer.h"


GridRenderSystem::GridRenderSystem(Controls* controls, Stats* stats, Kikan::Scene* scene) : _scene(scene), _stats(stats), _controls(controls){
    singleInclude(ColorQuadSprite);
}

void GridRenderSystem::update(double dt) {
    Kikan::Timer timer(&_stats->PERFORMANCE["Render Grid"], Kikan::Timer::Precision::MICRO);

    if(_controls->RENDER_MODE != Controls::RMT::GRID)
        return;

    update_color();
    update_vertices();

    _renderer->getBatch(1)->overrideVertices(_vertices, _indices);
}

void GridRenderSystem::setGrid(Grid* grid) {
    _grid = grid;

    _cellCount = _grid->getWidth() * _grid->getHeight();

    _rps.resize(_cellCount);
    _alphas.resize(_cellCount);

    gen_render_data();
}

void GridRenderSystem::update_color() {
    for (int i = 0; i < _cellCount; ++i) {
        _rps[i] = (float)_grid->cellCount(i) / 5.f;
        _alphas[i] = (float) _grid->cellCount(i) != 0 ? 1 : 0;
    }
}

void GridRenderSystem::update_vertices() {
    for (int i = 0; i < _cellCount; ++i) {

        if(i - (int)_grid->getWidth() - 1 < 0 || i + (int)_grid->getWidth() + 1 >= _cellCount)
            continue;

        float tl = (_rps[i + _grid->getWidth() - 1] + _rps[i + _grid->getWidth()] + _rps[i - 1] + _rps[i]) / 4.f;
        float tr = (_rps[i + _grid->getWidth()] + _rps[i + _grid->getWidth() + 1] + _rps[i] + _rps[i + 1]) / 4.f;
        float bl = (_rps[i - 1] + _rps[i] + _rps[i - _grid->getWidth() - 1] + _rps[i - _grid->getWidth()]) / 4.f;
        float br = (_rps[i] + _rps[i + 1] + _rps[i - _grid->getWidth()] + _rps[i - _grid->getWidth() + 1]) / 4.f;

        float atl = (_alphas[i + _grid->getWidth() - 1] + _alphas[i + _grid->getWidth()] + _alphas[i - 1] + _alphas[i]) / 2.f;
        float atr = (_alphas[i + _grid->getWidth()] + _alphas[i + _grid->getWidth() + 1] + _alphas[i] + _alphas[i + 1]) / 2.f;
        float abl = (_alphas[i - 1] + _alphas[i] + _alphas[i - _grid->getWidth() - 1] + _alphas[i - _grid->getWidth()]) / 2.f;
        float abr = (_alphas[i] + _alphas[i + 1] + _alphas[i - _grid->getWidth()] + _alphas[i - _grid->getWidth() + 1]) / 2.f;

        _vertices[4 * i + 0].color = glm::vec4(0, 0, 1 - tl, atl);
        _vertices[4 * i + 1].color = glm::vec4(0, 0, 1 - tr, atr);
        _vertices[4 * i + 2].color = glm::vec4(0, 0, 1 - br, abr);
        _vertices[4 * i + 3].color = glm::vec4(0, 0, 1 - bl, abl);
    }
}

void GridRenderSystem::gen_render_data() {
    _vertices.resize(_cellCount * 4);
    _indices.resize(_cellCount * 6);

    for (int y = 0; y < _grid->getHeight(); ++y) {
        for (int x = 0; x < _grid->getWidth(); ++x) {

            unsigned int i = x + _grid->getWidth() * y;
            glm::vec3 d = glm::vec3(x, y, -.1) * _grid->getSize();

            _vertices[4 * i + 0].position = glm::vec3(0, 1, 0) * _grid->getSize() + d;
            _vertices[4 * i + 1].position = glm::vec3(1, 1, 0) * _grid->getSize() + d;
            _vertices[4 * i + 2].position = glm::vec3(1, 0, 0) * _grid->getSize() + d;
            _vertices[4 * i + 3].position = glm::vec3(0, 0, 0) * _grid->getSize() + d;

            for (int j = 0; j < 4; ++j) {
                _vertices[4 * i + j].color = glm::vec4(0.8, .4, .2, 1.);
                _vertices[4 * i + j].textureCoords = glm::vec2(0);
                _vertices[4 * i + j].texture = -1;
            }

            _indices[6 * i + 0] = 0 + 4 * i;
            _indices[6 * i + 1] = 1 + 4 * i;
            _indices[6 * i + 2] = 2 + 4 * i;
            _indices[6 * i + 3] = 0 + 4 * i;
            _indices[6 * i + 4] = 2 + 4 * i;
            _indices[6 * i + 5] = 3 + 4 * i;
        }
    }
}

