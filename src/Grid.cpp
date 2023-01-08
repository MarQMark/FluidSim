#include "FluidSim/Grid.h"
#include <algorithm>

Grid::Grid(glm::vec2 pos, unsigned int w, unsigned int h, float size)
   : _pos(pos), _w(w), _h(h), _size(size) {

    _world_w = (float)_w * _size;
    _world_h = (float)_h * _size;

    //_grid.reserve(_w * _h);
    //for(int i = 0; i < _grid.size(); i++)
    //    _grid.at(i) = new std::vector<Particle*>();

    for(int i = 0; i < _w * _h; i++)
        _grid.push_back(new std::vector<Particle*>());
}

Grid::~Grid() {
    for (auto v : _grid)
        delete v;
}

void Grid::moveParticle(Particle* p) {
    std::vector<Particle*>* old_pos = _grid[grid_pos1D(p->ppos)];

    if(!old_pos->empty()){
        auto it = std::find(old_pos->begin(), old_pos->end(), p);
        if(it != old_pos->end())
            old_pos->erase(it);
    }

    unsigned int index = grid_pos1D(p->pos);
    p->index = index;
    _grid[index]->push_back(p);
}

void Grid::possibleNeighbours(std::vector<Particle *>& posNeigh, Particle* p) {
    glm::vec2 gpos;
    grid_pos2D(p->pos, gpos);

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            glm::vec2 i = gpos + glm::vec2(x, y);
            if(i.x >= 0 && i.y >= 0 && i.x < _w && i.y < _h){
                std::vector<Particle*>* cell = _grid[i.x + _w * i.y];
                for (Particle* posP : *cell) {
                    posNeigh.push_back(posP);
                }
            }
        }
    }
}

unsigned int Grid::grid_pos1D(glm::vec2 pos) {
    glm::vec2 translated = pos - _pos;
    int x = (int)((pos.x * (float)_w) / _world_w);
    int y = (int)((pos.y * (float)_h) / _world_h);

    x = std::max(x, 0);
    x = std::min(x, (int)_w);
    y = std::max(y, 0);
    y = std::min(y, (int)_h);

    return x + y * _w;
}

void Grid::grid_pos2D(glm::vec2 pos, glm::vec2& gpos) {
    glm::vec2 translated = pos - _pos;
    int x = (int)((pos.x * (float)_w) / _world_w);
    int y = (int)((pos.y * (float)_h) / _world_h);

    x = std::max(x, 0);
    x = std::min(x, (int)_w);
    y = std::max(y, 0);
    y = std::min(y, (int)_h);

    gpos.x = (float)x;
    gpos.y = (float)y;
}

