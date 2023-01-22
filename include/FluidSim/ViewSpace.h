#ifndef FLUIDSIM_VIEW_SPACE_H
#define FLUIDSIM_VIEW_SPACE_H

#include "Kikan/opengl/buffers/Texture2D.h"
#include "Controls.h"
#include "MapFile.h"

class ViewSpace{
public:
    explicit ViewSpace(Kikan::Texture2D* txt, MapFile* mapFile);
    ~ViewSpace();

    void render();

    float getWidth() const;
    float getHeight() const;

    float getZoom() const;

    Controls* getControls();

    void setMapFile(MapFile* mapFile);
private:
    Kikan::Texture2D* _txt;
    MapFile* _mf;

    Controls* _controls;

    float _zoom = 1.f;

    float _width = 1.f;
    float _height = 1.f;
};

#endif //FLUIDSIM_VIEW_SPACE_H
