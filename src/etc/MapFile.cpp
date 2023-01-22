#include "FluidSim/MapFile.h"
#include "stb_image/stb_image.h"

MapFile::MapFile(const std::string &path, float* progress, std::string* progress_msg) {
    int mapBPP;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* buff = stbi_load(path.c_str(), &_width, &_height, &mapBPP, 4);
    _txt = new Kikan::Texture2D(_width, _height, buff);

    std::string s(path);
    s.erase(s.end() - 4, s.end());
    std::string datafile(s + ".dat");
    if(progress && progress_msg)
        _df = new DistanceField(glm::vec2(0, 0), _width, _height, buff, datafile, progress, progress_msg);
    else
        _df = new DistanceField(glm::vec2(0, 0), _width, _height, buff, datafile);


    size_t pos = 0;
    while ((pos = s.find('/')) != std::string::npos) {
        s.erase(0, pos + 1);
    }
    _name = s;
}

Kikan::Texture2D *MapFile::getTexture() {
    return _txt;
}

DistanceField *MapFile::getDistanceField() {
    return _df;
}

MapFile::~MapFile() {
    delete _txt;
    delete _df;
}

int MapFile::getWidth() const {
    return _width;
}

int MapFile::getHeight() const {
    return _height;
}

std::string MapFile::getName() {
    return _name;
}
