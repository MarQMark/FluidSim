#ifndef FLUIDSIM_MAP_FILE_H
#define FLUIDSIM_MAP_FILE_H

#include <string>
#include "Kikan/opengl/buffers/Texture2D.h"
#include "DistanceField.h"

class MapFile{
public:
    explicit MapFile(const std::string& path, int* progress = nullptr, std::string* progress_msg = nullptr);
    ~MapFile();

    Kikan::Texture2D* getTexture();
    DistanceField* getDistanceField();

    int getWidth() const;
    int getHeight() const;
    std::string getName();

private:
    int _width = 0;
    int _height = 0;

    std::string _name;

    Kikan::Texture2D* _txt;

    DistanceField* _df;
};

#endif //FLUIDSIM_MAP_FILE_H
