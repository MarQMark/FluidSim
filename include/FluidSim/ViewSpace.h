#ifndef FLUIDSIM_VIEW_SPACE_H
#define FLUIDSIM_VIEW_SPACE_H

#include "Kikan/opengl/buffers/Texture2D.h"

class ViewSpace{
public:
    explicit ViewSpace(Kikan::Texture2D* txt);

    void render();

    float getWidth() const;
    float getHeight() const;

private:
    Kikan::Texture2D* _txt;

    float _width = 1.f;
    float _height = 1.f;
};

#endif //FLUIDSIM_VIEW_SPACE_H
