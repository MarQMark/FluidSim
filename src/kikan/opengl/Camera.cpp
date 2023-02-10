#include "Kikan/opengl/Camera.h"

namespace Kikan {
    glm::mat4x4 Camera::matrix() {
        return _matrix;
    }

    void Camera::scale(float w, float h) {
        _matrix = glm::scale(_matrix, glm::vec3(w, h, 1.0f));
    }

    void Camera::scale(glm::vec2 s) {
        _matrix = glm::scale(_matrix, glm::vec3(s.x, s.y, 1.0f));
    }

    void Camera::translate(float x, float y) {
        _matrix = glm::translate(_matrix, glm::vec3(x, y, 0));
    }

    void Camera::translate(glm::vec2 p) {
        _matrix = glm::translate(_matrix, glm::vec3(p.x, p.y, 0));
    }

    void Camera::rotate(float d) {
        _matrix = glm::rotate(_matrix, d, glm::vec3(0, 0, 1.));
    }

    void Camera::reset() {
        _matrix = glm::mat4x4(1.0f);
    }
}