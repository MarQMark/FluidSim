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
    //TODO: Find out faster way
    //std::vector<Particle*>* old_pos = _grid[grid_pos1D(p->ppos)];

    //if(!old_pos->empty()){
    //    auto it = std::find(old_pos->begin(), old_pos->end(), p);
    //    if(it != old_pos->end())
    //        old_pos->erase(it);
    //}

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

                posNeigh.reserve(posNeigh.size() + cell->size());
                posNeigh.insert(posNeigh.end(), cell->begin(), cell->end());

                if(x == 0 && y == 0){
                    for(int i2 = 0; i2 < posNeigh.size(); ++i2){
                        if(p->pos == posNeigh[i2]->pos){
                            posNeigh.erase(posNeigh.begin() + i2);
                            i2--;
                        }
                    }
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
    x = std::min(x, (int)_w - 1);
    y = std::max(y, 0);
    y = std::min(y, (int)_h - 1);

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

int Grid::count() {
    int count = 0;
    for (auto* cell : _grid) {
        count += cell->size();
    }

    return count;
}

void Grid::clear() {
    for (auto* cell : _grid) {
        cell->clear();
    }
}

unsigned int Grid::getWidth() const {
    return _w;
}

unsigned int Grid::getHeight() const {
    return _h;
}

float Grid::getSize() const {
    return _size;
}

unsigned int Grid::cellCount(int index) {
    return _grid[index % _grid.size()]->size();
}

