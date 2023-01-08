#ifndef KIKAN_VERTEX_BUFFER_H
#define KIKAN_VERTEX_BUFFER_H

#include <vector>
#include <memory>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Kikan/opengl/vertices/IVertex.h"

namespace Kikan {
    class VertexBuffer {
    public:
        explicit VertexBuffer(VertexBufferLayout* vbl, unsigned int vertexSize);
        ~VertexBuffer();

        void addVertices(char* vertices, unsigned int count) const;

        void bind();

    private:
        GLuint _id = 0;
        VertexBufferLayout* _vbl;

        unsigned int _vertex_size;

        GLint _offset = 0;
    };
}

#endif //KIKAN_VERTEX_BUFFER_H
