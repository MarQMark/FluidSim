#ifndef KIKAN_TEXTURE2D_SPRITE_H
#define KIKAN_TEXTURE2D_SPRITE_H

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "IComponent.h"

namespace Kikan {
    struct Texture2DSprite : IComponent {
        glm::vec2 points[4];
        glm::vec4 color;
        GLuint textureID;
        float layer;
    };
}

#endif //KIKAN_TEXTURE2D_SPRITE_H
