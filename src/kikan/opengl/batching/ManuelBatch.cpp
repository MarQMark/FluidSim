#include "Kikan/opengl/batching/ManuelBatch.h"

#include "Kikan/opengl/buffers/VertexBuffer.h"
#include "Kikan/opengl/buffers/IndexBuffer.h"

namespace Kikan {
    ManuelBatch::ManuelBatch(VertexBufferLayout *vbl, GLuint vertexSize) :
     _vbl(vbl) {
        _vertex_space.size = vertexSize;
    }

    ManuelBatch::~ManuelBatch() {
        delete _vertex_space.data;
    }

    void ManuelBatch::render() {
        //Vertex buffers
        VertexBuffer vb(_vbl, _vertex_space.size);
        vb.addVertices(_vertex_space.data, _vertex_space.count);
        vb.bind();

        //Index buffers
        IndexBuffer ib(_indices);
        ib.bind();

        //bind all textures
        for (auto t : _textures) {
            glActiveTexture(GL_TEXTURE0 + t.first);
            glBindTexture(GL_TEXTURE_2D, t.second);
        }

        //draw
        glDrawElements(GL_TRIANGLES, (GLsizei) _indices.size(), GL_UNSIGNED_INT, nullptr);
    }

    void ManuelBatch::addTexture(GLsizei id, int slot) {
        _textures[slot] = id;
    }

    void ManuelBatch::removeTexture(GLsizei id) {
        _textures.erase(id);
    }
}