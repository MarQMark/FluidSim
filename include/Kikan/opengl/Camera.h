#ifndef KIKAN_CAMERA_H
#define KIKAN_CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace Kikan {
    class Camera {
    public:
        Camera() {
            reset();
        }

        glm::mat4x4 matrix();

        void scale(float w, float h);
        void scale(glm::vec2 s);

        void translate(float x, float y);
        void translate(glm::vec2 p);

        void rotate(float d);

        void reset();
    private:

        glm::mat4x4 _matrix{};
    };
}

#endif //KIKAN_CAMERA_H
