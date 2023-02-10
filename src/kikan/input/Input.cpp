#include "Kikan/input/Input.h"

namespace Kikan {
     Input *Input::create(GLFWwindow* window) {
        auto* input = new Input(window);

        glfwSetWindowUserPointer(window, input);

        // setup Mouse Button callback
        auto mouse_btn = [](GLFWwindow* w, int b, int a, int m){
            static_cast<Input*>(glfwGetWindowUserPointer(w))->mouse_btn_callback( b, a, m);
        };
        glfwSetMouseButtonCallback(window, mouse_btn);

        // setup Mouse Position callback
        auto mouse_pos = [](GLFWwindow* w, double x, double y){
            static_cast<Input*>(glfwGetWindowUserPointer(w))->mouse_pos_callback(x, y);
        };
        glfwSetCursorPosCallback(window, mouse_pos);

        // setup Key callback
        auto key = [](GLFWwindow* w, int k, int s, int a, int m){
            static_cast<Input*>(glfwGetWindowUserPointer(w))->key_callback( k, s, a, m);
        };
        glfwSetKeyCallback(window, key);

        return input;
    }

    Input::Input(GLFWwindow* window) {
        // init maps
        for (int i = -1; i < Key::LAST; ++i) {
            _keys[i] = false;
        }
        for (int i = 0; i < Mouse::BUTTON_LAST; ++i) {
            _m_keys[i] = false;
        }
    }

    void Input::mouse_btn_callback(int btn, int action, int mods) {
        _m_keys[btn] = (action == GLFW_PRESS);
    }

    void Input::mouse_pos_callback(double x, double y) {
        _mouse_x = x;
        _mouse_y = y;
    }

    void Input::key_callback(int key, int scancode, int action, int mods) {
        _keys[key] = (action == GLFW_PRESS);
    }

    bool Input::keyPressed(Key k) {
        return _keys[k];
    }

    double Input::mouseX() const {
        return _mouse_x;
    }

    double Input::mouseY() const {
        return _mouse_y;
    }

    void Input::mouseP(glm::vec2 &pos) const {
        pos.x = (float)_mouse_x;
        pos.y = (float)_mouse_y;
    }

    bool Input::mousePressed(Mouse m) {
        return _m_keys[m];
    }


}