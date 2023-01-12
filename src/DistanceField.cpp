#include "FluidSim/DistanceField.h"

DistanceField::DistanceField(glm::vec2 pos, int width, int height, unsigned char* data) :
    _pos(pos), _width(width), _height(height), _data(data) , _grid(std::vector<Field*>(width * height)){

    // Pain

}

DistanceField::~DistanceField() {
    for (Field* field : _grid)
        delete field;
}

float DistanceField::distance(glm::vec2 pos) {
    return 0;
}

