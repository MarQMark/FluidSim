#include <iostream>
#include <fstream>
#include "FluidSim/DistanceField.h"
#include <thread>

#define KERNEL_RADIUS 4

DistanceField::DistanceField(glm::vec2 pos, int width, int height, unsigned char* data, std::string& file) :
    _pos(pos), _width(width), _height(height), _data(data) , _grid(std::vector<Field*>(width * height)){

    _file_name = file;

    init();
}

DistanceField::DistanceField(glm::vec2 pos, int width, int height, unsigned char *data, std::string &file, float *progress, std::string *progress_msg) :
        _pos(pos), _width(width), _height(height), _data(data) , _grid(std::vector<Field*>(width * height)) {

    // This is kinda dumb. Too bad!
    _file_name = file;
    _progress = progress;
    _progress_msg = progress_msg;

    std::thread t(&DistanceField::init, this);
    t.detach();
}

void DistanceField::init(){
    if(_progress != nullptr)
        *_progress = 0;

    std::ifstream f(_file_name.c_str(), std::ios::out | std::ios::binary);
    if(f.good())
        load_file(f);
    else
        generate_field(_file_name);
    f.close();

    if(_progress != nullptr)
        *_progress = 1;
}

void DistanceField::load_file(std::ifstream& f) {
    if(_progress_msg != nullptr)
        *_progress_msg = "Loading from file";

    for (int i = 0; i < _width * _height; ++i) {
        _grid[i] = new Field(0);
        f.read((char*) _grid[i], sizeof(Field));

        if(_progress != nullptr)
            *_progress = (float)i / (float)(_width * _height);
    }
    if(!f.good())
        std::cout << "[ERROR] Failed to read distance field data from file" << std::endl;
}

void DistanceField::generate_field(std::string &file) {
    // Calculate Distance
    if(_progress_msg != nullptr)
        *_progress_msg = "Generating distance Field";

    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            // Starting Pos is Black -> Border
            if(color(x, y) == 0){
                calc_dist(x, y, 255);
            }
            //Starting Pos is White -> Free Space
            else{
                calc_dist(x, y, 0);
            }
        }

        if(_progress != nullptr)
            *_progress = (float)x / (float)_width;
    }


    // Calculate Normal
    if(_progress_msg != nullptr)
        *_progress_msg = "Generating Normals";

    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {

            //glm::vec2 normal = glm::vec2(x, y) -  glm::vec2(_width / 2, _height / 2);
            //_grid[x + _width * y]->normal = glm::normalize(normal);

            glm::vec2 center(x, y);
            glm::vec2 normal(0,0);

            for(int dx = -KERNEL_RADIUS; dx <= KERNEL_RADIUS; dx++){
                for(int dy = -KERNEL_RADIUS; dy <= KERNEL_RADIUS; dy++) {
                    if(
                            x + dx >= 0 && x + dx < _width &&
                            y + dy >= 0 && y + dy < _height &&
                            !(dx == 0 && dy == 0))
                    {
                        normal += (float)_grid[(x + dx) + _width * (y + dy)]->distance * glm::normalize( glm::vec2(x + dx, y + dy) - center);
                    }
                }
            }

            if(!(normal.x == 0 && normal.y == 0))
                normal = glm::normalize(normal);

            _grid[x + _width * y]->normal = normal;
        }

        if(_progress != nullptr)
            *_progress =(float)x / (float)_width;
    }

    // Write to file
    std::ofstream f(file, std::ios::out | std::ios::binary);
    if(!f){
        std::cout << "[ERROR] Cannot create file \"" << file <<"\"" << std::endl;
        return;
    }

    for (int i = 0; i < _width * _height; ++i) {
        f.write((char*) _grid[i], sizeof(Field));
    }
    if(!f.good()){
        std::cout << "[ERROR] failed to write to file \"" << file <<"\"" << std::endl;
        return;
    }
    f.close();
}


bool DistanceField::color_check(int x, int y, glm::vec2 pos, unsigned char col, int i){
    if(color(pos) == col){
        _grid[x + _width *  y] = new Field((col == 0) ? -(i + 1) : i);
        return true;
    }
    return false;
}

void DistanceField::calc_dist(int x, int y, unsigned char target_col) {
    glm::vec2 curr(x, y);
    for (int i = 0; color(curr) != target_col && i < std::max(_width, _height); ++i) {
        if(*_progress_msg == "KILL"){
            exit(0);
        }

        // Start
        curr += glm::vec2(0, 1);
        if(color_check(x, y, curr, target_col, i)) break;

        //Top to Right
        while((int)curr.y > y){
            curr += glm::vec2(1, -1);
            if(color_check(x, y, curr, target_col, i)) break;
        }
        if(color(curr) == target_col) break;

        //Right to Bottom
        while((int)curr.x > x){
            curr += glm::vec2(-1, -1);
            if(color_check(x, y, curr, target_col, i)) break;
        }
        if(color(curr) == target_col) break;

        //Bottom to Left
        while((int)curr.y < y){
            curr += glm::vec2(-1, 1);
            if(color_check(x, y, curr, target_col, i)) break;
        }
        if(color(curr) == target_col) break;

        //Left to Top
        while((int)curr.x < x){
            curr += glm::vec2(1, 1);
            if(color_check(x, y, curr, target_col, i)) break;
        }
        if(color(curr) == target_col) break;
    }
}

DistanceField::~DistanceField() {
    for (Field* field : _grid)
        delete field;
}

int DistanceField::distance(glm::vec2 pos) {
    pos -= _pos;
    if(std::isnan(pos.x) || std::isnan(pos.y))
        return -10000;

    pos.x = std::max(pos.x, 0.f);
    pos.x = std::min(pos.x, (float)_width - 1.f);
    pos.y = std::max(pos.y, 0.f);
    pos.y = std::min(pos.y, (float)_height - 1.f);

    int index = (int)pos.x + _width * (int)pos.y;
    return _grid[index]->distance;
}


void DistanceField::normal(glm::vec2 pos, glm::vec2& normal) {
    pos -= _pos;
    pos.x = std::max(pos.x, 0.f);
    pos.x = std::min(pos.x, (float)_width - 1.f);
    pos.y = std::max(pos.y, 0.f);
    pos.y = std::min(pos.y, (float)_height - 1.f);

    int index = (int)pos.x + _width * (int)pos.y;
    normal = _grid[index]->normal;
}

unsigned char DistanceField::color(int x, int y) {
    x = std::max(x, 0);
    x = std::min(x, _width - 1);
    y = std::max(y, 0);
    y = std::min(y, _height - 1);

    return _data[(x + _width * y) * 4];
}

unsigned char DistanceField::color(glm::vec2 pos) {
    return color((int)pos.x , (int)pos.y);
}

int DistanceField::getWidth() {
    return _width;
}

int DistanceField::getHeight() {
    return _height;
}


