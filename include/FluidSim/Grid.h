#ifndef FLUIDSIM_GRID_H
#define FLUIDSIM_GRID_H

#include "glm/glm.hpp"
#include "vector"
#include "FluidSim/Particle.h"

class Grid {
public:
    Grid(glm::vec2 pos, unsigned int w, unsigned int h, float size);
    ~Grid();

    void clear();
    void moveParticle(Particle* p);
    void possibleNeighbours(std::vector<Particle*>& posNeigh, Particle* p);
    int count();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    float getSize() const;
    unsigned int cellCount(int index);
private:
    glm::vec2 _pos;
    unsigned int _w;
    unsigned int _h;
    float _size;
    float _world_w;
    float _world_h;


    std::vector<std::vector<Particle*>*> _grid;

    unsigned int grid_pos1D(glm::vec2 pos);
    void grid_pos2D(glm::vec2 pos, glm::vec2& gpos);
};


#endif //FLUIDSIM_GRID_H
