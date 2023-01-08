#ifndef KIKAN_TEXTURE2D_H
#define KIKAN_TEXTURE2D_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Kikan{
    class Texture2D {
    public:
        Texture2D(GLsizei width, GLsizei height, float data[]);
        ~Texture2D();

        GLuint get() const;

        void bind(GLuint slot) const;
        static void unbind();
    private:
        GLuint _id;
        GLsizei _width;
        GLsizei _height;
    };
}

#endif //KIKAN_TEXTURE2D_H
