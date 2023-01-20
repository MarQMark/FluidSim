#ifndef FLUIDSIM_DISTANCE_FIELD_H
#define FLUIDSIM_DISTANCE_FIELD_H

#include "glm/glm.hpp"
#include "vector"

class DistanceField {
private:
    struct Field{
        explicit Field(int dist) : distance(dist) {};

        int distance;
        glm::vec2 normal{};
    };

public:
    DistanceField(glm::vec2 pos, int width, int height, unsigned char* data, std::string& file);
    ~DistanceField();

    int distance(glm::vec2 pos);
    void normal(glm::vec2 pos, glm::vec2& normal);

    int getWidth();
    int getHeight();

private:
    glm::vec2 _pos;

    unsigned char* _data;
    int _width;
    int _height;

    std::vector<Field*> _grid;

    void load_file(std::ifstream& f);
    void generate_field(std::string& file);

    void calc_dist(int x, int y, unsigned char target_col);
    bool color_check(int x, int y, glm::vec2 pos, unsigned char col, int i);

    unsigned char color(int x, int y);
    unsigned char color(glm::vec2 pos);
};


#endif //FLUIDSIM_DISTANCE_FIELD_H
