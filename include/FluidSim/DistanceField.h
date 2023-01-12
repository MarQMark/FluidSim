#ifndef FLUIDSIM_DISTANCE_FIELD_H
#define FLUIDSIM_DISTANCE_FIELD_H

#include "glm/glm.hpp"
#include "vector"

class DistanceField {
private:
    struct Field{
        int distance;
        glm::vec2 normal;
    };

public:
    DistanceField(glm::vec2 pos, int width, int height, unsigned char* data);
    ~DistanceField();

    float distance(glm::vec2 pos);
private:
    glm::vec2 _pos;

    unsigned char* _data;
    int _width;
    int _height;

    std::vector<Field*> _grid;
};


#endif //FLUIDSIM_DISTANCE_FIELD_H
