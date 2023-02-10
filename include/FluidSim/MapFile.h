#ifndef FLUIDSIM_MAP_FILE_H
#define FLUIDSIM_MAP_FILE_H

#include <string>
#include "Kikan/opengl/buffers/Texture2D.h"
#include "DistanceField.h"

class MapFile{
public:
    explicit MapFile(const std::string& path, float* progress = nullptr, std::string* progress_msg = nullptr);
    ~MapFile();

    Kikan::Texture2D* getIcon();
    Kikan::Texture2D* getTexture();
    Kikan::Texture2D* getForeground();
    DistanceField* getDistanceField();

    int getWidth() const;
    int getHeight() const;

    std::string getName();

private:
    int _width = 0;
    int _height = 0;

    std::string _name;

    Kikan::Texture2D* _icon;
    Kikan::Texture2D* _txt;
    Kikan::Texture2D* _fg = nullptr;

    DistanceField* _df;
};

#endif //FLUIDSIM_MAP_FILE_H
