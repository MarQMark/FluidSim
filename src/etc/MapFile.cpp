#include "FluidSim/MapFile.h"
#include "stb_image/stb_image.h"

MapFile::MapFile(const std::string &path, float* progress, std::string* progress_msg) {

    // Load Data Image
    std::string mapImg(path + ".png");
    int mapImgBPP;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* buff = stbi_load(mapImg.c_str(), &_width, &_height, &mapImgBPP, 4);
    _icon = new Kikan::Texture2D(_width, _height, buff);


    // Load Background Image
    std::string bgImg(path + "_bg.png");
    int bgImgBPP, bgWidth, bgHeight;
    unsigned char* bgBuff = stbi_load(bgImg.c_str(), &bgWidth, &bgHeight, &bgImgBPP, 4);
    if(bgBuff)
        _txt = new Kikan::Texture2D(bgWidth, bgHeight, bgBuff);
    else
        _txt = new Kikan::Texture2D(_width, _height, buff);


    // Load Background Image
    std::string fgImg(path + "_fg.png");
    int fgImgBPP, fgWidth, fgHeight;
    unsigned char* fgBuff = stbi_load(fgImg.c_str(), &fgWidth, &fgHeight, &fgImgBPP, 4);
    if(fgBuff)
        _fg = new Kikan::Texture2D(fgWidth, fgHeight, fgBuff);


    // Generate/ Load Distance Field
    std::string datafile(path + ".dat");
    if(progress && progress_msg)
        _df = new DistanceField(glm::vec2(0, 0), _width, _height, buff, datafile, progress, progress_msg);
    else
        _df = new DistanceField(glm::vec2(0, 0), _width, _height, buff, datafile);


    // Get Name
    std::string s(path);
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
    delete _icon;
    delete _txt;
    delete _fg;
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

Kikan::Texture2D *MapFile::getForeground() {
    return _fg;
}

Kikan::Texture2D *MapFile::getIcon() {
    return _icon;
}
