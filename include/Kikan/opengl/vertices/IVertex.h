#ifndef KIKAN_IVERTEX_H
#define KIKAN_IVERTEX_H

#include "GL/glew.h"
#include "Kikan/opengl/buffers/VertexBufferLayout.h"

namespace Kikan {
    struct IVertex {
    public:
        /*
         * When auto-batching you set the textureSlot as the texture ID.
         * That ID gets translated into a textureSlot
         */
        float texture{};
    };
}
#endif //KIKAN_IVERTEX_H
