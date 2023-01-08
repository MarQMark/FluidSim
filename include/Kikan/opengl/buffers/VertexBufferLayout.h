#ifndef KIKAN_VERTEX_BUFFER_LAYOUT_H
#define KIKAN_VERTEX_BUFFER_LAYOUT_H

#include <vector>
#include "GL/glew.h"

namespace Kikan {
    struct VertexBufferElement{
        unsigned int type;
        GLint count;
        unsigned char normalized;

        static GLsizei getTypeSize(unsigned int type){
            switch (type) {
                case GL_FLOAT:
                case GL_UNSIGNED_INT:
                    return 4;
                case GL_UNSIGNED_BYTE:
                    return 1;
                default:
                    return 0;
            }
        }
    };

    template<typename T>
    struct always_false : std::false_type
    { };

    template<typename T>
    struct identity { typedef T type; };

    class VertexBufferLayout{
    public:
        VertexBufferLayout() = default;

        template<typename T>
        void add(GLint count){
            add(count, identity<T>());
        }

        inline GLsizei getStride() const {return _stride;}
        inline std::vector<VertexBufferElement> getElements() const& {return _elements;}

    private:
        template<typename T>
        void add(GLint count, identity<T>){
            static_assert(always_false<T>::value);
        }
        void add(GLint count, identity<float>){
            _elements.push_back({GL_FLOAT, count, GL_FALSE});
            _stride += count * VertexBufferElement::getTypeSize(GL_FLOAT);
        }
        void add(GLint count, identity<unsigned int>){
            _elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
            _stride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_INT);
        };
        void add(GLint count, identity<unsigned char>){
            _elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
            _stride += count * VertexBufferElement::getTypeSize(GL_UNSIGNED_BYTE);
        };

        std::vector<VertexBufferElement> _elements;
        GLsizei _stride = 0;
    };
}

#endif //KIKAN_VERTEX_BUFFER_LAYOUT_H
