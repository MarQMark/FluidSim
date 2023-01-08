#ifndef KIKAN_BATCH_H
#define KIKAN_BATCH_H

#include "Kikan/opengl/buffers/VertexBuffer.h"

namespace Kikan {
    class Batch {
    public:
        Batch() = default;

        virtual void bind() = 0;
        static void unbind();

        virtual void render();
    protected:
        GLsizei _last_vertex = 0;
    };
}

#endif //KIKAN_BATCH_H
