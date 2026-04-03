#pragma once

#include <GLFW/glfw3.h>

namespace autobot::board {

class main_window {
public:
    main_window();
    ~main_window();

    void run();

private:
    GLFWwindow* m_window;
};

}
