#include "Kikan/opengl/vertices/VertexRegistry.h"

namespace Kikan {
    int VertexRegistry::_last_signature = 0;
    std::map<std::string, unsigned int> VertexRegistry::_vertex_signatures;
    std::map<unsigned int, VertexBufferLayout *> VertexRegistry::_vertex_layouts;
}