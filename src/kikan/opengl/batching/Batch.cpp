#include "Kikan/opengl/batching/Batch.h"

namespace Kikan {
    void Batch::render() {
        //glDrawElements(GL_TRIANGLES, _last_vertex, GL_UNSIGNED_SHORT, );
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Batch::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}