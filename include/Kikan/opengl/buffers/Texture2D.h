#ifndef KIKAN_TEXTURE2D_H
#define KIKAN_TEXTURE2D_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Kikan{
    class Texture2D {
    public:
        Texture2D(GLsizei width, GLsizei height, float data[], GLenum filter = GL_NEAREST);
        Texture2D(GLsizei width, GLsizei height, unsigned char data[], GLenum filter = GL_NEAREST);
        ~Texture2D();

        GLuint get() const;

        void set(GLsizei width, GLsizei height, float data[]);
        void set(GLsizei width, GLsizei height, unsigned char data[]);

        void bind(GLuint slot = 0) const;
        static void unbind();
    private:
        void gen(GLenum filter);

        GLuint _id;
        GLsizei _width;
        GLsizei _height;
    };
}

#endif //KIKAN_TEXTURE2D_H
