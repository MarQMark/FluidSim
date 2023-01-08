#ifndef KIKAN_VERTEX_REGISTRY_H
#define KIKAN_VERTEX_REGISTRY_H

#include "map"
#include "string"
#include "typeinfo"
#include "Kikan/opengl/buffers/VertexBufferLayout.h"

namespace Kikan {
    class VertexRegistry {
    public:

        template<class T>
        static unsigned int getSignature() {
            std::string sig = typeid(T).name();

            if (_vertex_signatures.count(sig))
                return _vertex_signatures[sig];

            _vertex_signatures.insert(std::make_pair(sig, _last_signature++));
            return _last_signature - 1;
        }

        template<class T>
        static void addLayout(VertexBufferLayout *vbl) {
            unsigned int signature = getSignature<T>();

            if (_vertex_layouts[signature] != nullptr)
                delete _vertex_layouts[signature];

            _vertex_layouts[signature] = vbl;
        }

        template<class T>
        static VertexBufferLayout *getLayout() {
            return _vertex_layouts[getSignature<T>()];
        }

    private:
        static std::map<std::string, unsigned int> _vertex_signatures;
        static std::map<unsigned int, VertexBufferLayout *> _vertex_layouts;
        static int _last_signature;
    };
}

#endif //KIKAN_VERTEX_REGISTRY_H
