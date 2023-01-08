#ifndef KIKAN_DEFAULT_VERTEX_H
#define KIKAN_DEFAULT_VERTEX_H

#include "glm/glm.hpp"
#include "IVertex.h"

namespace Kikan {
    struct DefaultVertex : IVertex {
        glm::vec3 position;
        glm::vec2 textureCoords;
        glm::vec4 color;

        static VertexBufferLayout *getLayout() {
            auto layout = new VertexBufferLayout;

            layout->add<float>(1);
            layout->add<float>(3);
            layout->add<float>(2);
            layout->add<float>(4);

            return layout;
        }
    };
}

#endif //KIKAN_DEFAULT_VERTEX_H
