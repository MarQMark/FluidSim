#ifndef KIKAN_MANUEL_BATCH_H
#define KIKAN_MANUEL_BATCH_H

#include <map>
#include <iostream>
#include <cstring>

#include "Kikan/opengl/buffers/VertexBufferLayout.h"


namespace Kikan {
    class ManuelBatch {
    public:
        ManuelBatch(VertexBufferLayout* vbl, GLuint vertexSize);
        ~ManuelBatch();

        template<class T>
        void addVertices(std::vector<T>& vertices, std::vector<GLuint>& indices);

        template<class T>
        void updateVertices(std::vector<T> &vertices, unsigned int vbegin, unsigned int vend, std::vector<GLuint> &indices, unsigned int ibegin, unsigned int iend);

        template<class T>
        void overrideVertices(std::vector<T>& vertices, std::vector<GLuint>& indices);

        void removeVertices();

        void addTexture(GLsizei id, int slot);
        void removeTexture(GLsizei id);

        void render();

    private:
        struct VertexSpace {
            char* data = nullptr;
            unsigned int count = 0;
            GLuint size = 0;
        };

        VertexBufferLayout* _vbl;
        VertexSpace _vertex_space;

        std::map<unsigned int, int> _textures;

        GLuint _next_index = 0;
        std::vector<GLuint> _indices;
    };

    template<class T>
    void ManuelBatch::updateVertices(std::vector<T> &vertices, unsigned int vbegin, unsigned int vend, std::vector<GLuint> &indices, unsigned int ibegin, unsigned int iend) {
        if(sizeof(T) != _vertex_space.size){
            std::cout << "[MANUEL BATCH] Cannot add vertices of different size to batch" << std::endl;
            return;
        }

        //allocate enough space for all new vertices
        unsigned int oldSize = _vertex_space.count;
        _vertex_space.count -= (vend - vbegin) + vertices.size();

        char *data = static_cast<char *>(malloc(_vertex_space.size * _vertex_space.count));
        if(data == nullptr){
            std::cout << "[MANUEL BATCH] Error allocating memory" << std::endl;
            return;
        }

        //handle old data
        //copy before updated segment
        memcpy(data, _vertex_space.data, _vertex_space.size * vbegin);
        //copy after updated segment
        if(oldSize - vend > 0)
            memcpy(data + (vbegin + vertices.size()) * _vertex_space.size, _vertex_space.data + vend * _vertex_space.size, oldSize - vend);
        free(_vertex_space.data);

        //copy new data
        _vertex_space.data = memcpy(data + vbegin * _vertex_space.size, vertices.data(), _vertex_space.size);

        //copy indices
        uint64_t iOldSize = _indices.size();
        uint64_t iSize = _indices.size() - (iend - ibegin) + indices.size();
        _indices.reserve(iSize);
        //if new vertices inserted into middle move back part of original indices to back of new size
        if(iOldSize - iend > 0)
            _indices.insert(_indices.begin() + ibegin + (unsigned int)indices.size(), _indices.begin() + iend, _indices.begin() + (unsigned int)iOldSize);
        //insert new indices
        _indices.insert(_indices.begin() + ibegin, indices.begin(), indices.end());
    }

    template<class T>
    void ManuelBatch::addVertices(std::vector<T> &vertices, std::vector<GLuint> &indices) {
        if(sizeof(T) != _vertex_space.size){
            std::cout << "[MANUEL BATCH] Cannot add vertices of different size to batch" << std::endl;
            return;
        }

        //allocate enough space for all new vertices
        char *data = static_cast<char *>(malloc(_vertex_space.size * (_vertex_space.count + vertices.size())));
        if(data == nullptr){
            std::cout << "[MANUEL BATCH] Error allocating memory" << std::endl;
            return;
        }

        //handle old data
        memcpy(data, _vertex_space.data, _vertex_space.size * _vertex_space.count);
        free(_vertex_space.data);

        //copy new data
        memcpy(data + _vertex_space.count * _vertex_space.size, vertices.data(), _vertex_space.size * vertices.size());

        _vertex_space.count += vertices.size();
        _vertex_space.data = data;

        _indices.insert(_indices.end(), indices.begin(), indices.end());
    }

    template<class T>
    void ManuelBatch::overrideVertices(std::vector<T> &vertices, std::vector<GLuint> &indices) {
        if(sizeof(T) != _vertex_space.size){
            std::cout << "[MANUEL BATCH] Cannot add vertices of different size to batch" << std::endl;
            return;
        }

        //allocate enough space for all new vertices
        char *data = static_cast<char *>(malloc(_vertex_space.size * vertices.size()));
        if(data == nullptr){
            std::cout << "[MANUEL BATCH] Error allocating memory" << std::endl;
            return;
        }

        free(_vertex_space.data);

        //copy new data
        memcpy(data, vertices.data(), _vertex_space.size * vertices.size());

        _vertex_space.count = vertices.size();
        _vertex_space.data = data;

        _indices = indices;
    }
}

#endif //KIKAN_MANUEL_BATCH_H
