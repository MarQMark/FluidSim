#ifndef KIKAN_AUTO_BATCH_H
#define KIKAN_AUTO_BATCH_H

#include "Batch.h"

namespace Kikan {
    class AutoBatch{
    public:
         explicit AutoBatch(VertexBufferLayout* vbl, GLuint vertexSize);
        ~AutoBatch();

        int addVertices(std::vector<IVertex*>& vertices);
        int addVertices(std::vector<IVertex*>& vertices, std::vector<GLuint>& indices);

        void render();

    private:
        int add_vertices(std::vector<IVertex*>& vertices);

        struct VertexSpace {
            char* data = nullptr;
            unsigned int count = 0;
            GLuint size = 0;
        };

        int _max_texture_units = 4;
        float* _texture_ids;
        int _last_slot = 0;
        int find_texture(float texture);

        VertexBufferLayout* _vbl;
        VertexSpace _vertex_space;

        GLuint _next_index = 0;
        std::vector<GLuint> _indices;
    };
}

#endif //KIKAN_AUTO_BATCH_H
