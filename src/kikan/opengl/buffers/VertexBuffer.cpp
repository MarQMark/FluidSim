#include "Kikan/opengl/buffers/VertexBuffer.h"

namespace Kikan {
    VertexBuffer::VertexBuffer(VertexBufferLayout *vbl, unsigned int vertexSize) {
        _vbl = vbl;
        _vertex_size = vertexSize;
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ARRAY_BUFFER, _id);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &_id);
    }

    void VertexBuffer::addVertices(char *vertices, unsigned int count) const {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferData(GL_ARRAY_BUFFER, _vertex_size * count, vertices, GL_STATIC_DRAW);
    }

    void VertexBuffer::bind() {
        glBindBuffer(GL_ARRAY_BUFFER, _id);

        //load layout
        const std::vector<VertexBufferElement> &elements = _vbl->getElements();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); ++i) {
            const VertexBufferElement &element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.normalized,
                                  _vbl->getStride(), (const void *) (size_t) offset);
            offset += element.count * VertexBufferElement::getTypeSize(element.type);
        }
    }
}