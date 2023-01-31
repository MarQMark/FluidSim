#include "FluidSim/GridRenderSystem.h"
#include "FluidSim/ColorQuadSprite.h"
#include "Kikan/util/Timer.h"


GridRenderSystem::GridRenderSystem(Controls* controls, Stats* stats) : _stats(stats), _controls(controls){
    singleInclude(ColorQuadSprite);

    _texture = new Kikan::Texture2D(0, 0, (float*)nullptr);
}

void GridRenderSystem::update(double dt) {
    Kikan::Timer timer(&_stats->PERFORMANCE["Render Grid"], Kikan::Timer::Precision::MICRO);

    if(_controls->RENDER_MODE != Controls::RMT::GRID)
        return;

    float color[_cellCount * 4];
    for (int i = 0; i < _cellCount; ++i) {
        float c = (float)_grid->cellCount(i) / 10.f;
        if(i - (int)_grid->getWidth() - 1 >= 0 && i + (int)_grid->getWidth() + 1 < _cellCount){
            c += (float)_grid->cellCount(i - (int)_grid->getWidth() - 1) / 10.f;
            c += (float)_grid->cellCount(i - (int)_grid->getWidth()) / 10.f;
            c += (float)_grid->cellCount(i - (int)_grid->getWidth() + 1) / 10.f;
            c += (float)_grid->cellCount(i - 1) / 10.f;
            c += (float)_grid->cellCount(i + 1) / 10.f;
            c += (float)_grid->cellCount(i + (int)_grid->getWidth() - 1) / 10.f;
            c += (float)_grid->cellCount(i + (int)_grid->getWidth()) / 10.f;
            c += (float)_grid->cellCount(i + (int)_grid->getWidth() + 1) / 10.f;
            c /= 9;
        }
        color[4 * i + 0] = 0;
        color[4 * i + 1] = 0;
        color[4 * i + 2] = 1 - c;
        color[4 * i + 3] = c == 0 ? 0 : 1;
    }
    _texture->set((int)_grid->getWidth(), (int)_grid->getHeight(), color);
    _renderer->getBatch(1)->addTexture((int)_texture->get(), 0);
}

void GridRenderSystem::setGrid(Grid* grid) {
    _grid = grid;
    _cellCount = _grid->getWidth() * _grid->getHeight();
}

GridRenderSystem::~GridRenderSystem() {
    Kikan::IRenderSystem::~IRenderSystem();
    delete _texture;
}

