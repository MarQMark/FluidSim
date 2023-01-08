#ifndef KIKAN_INPUT_H
#define KIKAN_INPUT_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Kikan {
    class Input {
    public:
        Input(GLFWwindow* window);

    private:
        void mouse_btn_callback(int btn, int action, int mods);
        void mouse_pos_callback(double x, double y);
        void key_callback(int key, int scancode, int action, int mods);
    };
}

#endif //KIKAN_INPUT_H
