#ifndef KIKAN_INDEX_BUFFER_H
#define KIKAN_INDEX_BUFFER_H

#include <vector>
#include "GL/glew.h"

namespace Kikan {
    class IndexBuffer {
    public:
        explicit IndexBuffer(std::vector<GLuint>& indices);
        ~IndexBuffer();

        void bind() const;
        static void unbind();

    private:
        GLuint _id = 0;
    };
}

#endif //KIKAN_INDEX_BUFFER_H
