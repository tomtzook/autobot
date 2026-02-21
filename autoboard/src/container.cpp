
#include "container.h"

namespace ui {

container::container()
    : m_obsr_storage()
    , m_obsr_tree(m_obsr_storage)
    , m_plot_windows()
    , m_canvas_windows()
    , m_renderer3d()
    , m_camera(glm::radians(45.0f), 1280.0f / 720.0f, 0.01f, 100.0f)
    , m_world3d() {
    create_base();
}

void container::update(GLFWwindow* window) {
    for (auto& win : m_plot_windows) {
        win.update();
    }
    for (auto& win : m_canvas_windows) {
        win.update();
    }

    m_world3d.update();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        m_camera.move_forward(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        m_camera.move_backward(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        m_camera.move_right(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        m_camera.move_left(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        m_camera.rotate_pitch(glm::radians(1.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        m_camera.rotate_pitch(glm::radians(-1.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        m_camera.rotate_yaw(glm::radians(-1.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        m_camera.rotate_yaw(glm::radians(1.0f));
    }
}

void container::draw_ui() {
    {
        ImGui::Begin("obsr");
        m_obsr_tree.draw();
        ImGui::End();
    }

    for (auto& win : m_plot_windows) {
        win.draw();
    }
    for (auto& win : m_canvas_windows) {
        win.draw();
    }
}

void container::render3d() {
    /*auto cube = ui::render::cube_mesh(1, 1, 1);
    cube.color(gl::color(255, 0, 0, 255));
    const auto transformation_cube = transformation(glm::vec3(0.1f, 0.0f, -3.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f));
    renderer.render(transformation_cube, cube);*/

    auto renderer = m_renderer3d.start(m_camera.projection(), m_camera.view());
    m_world3d.render(renderer);
}

void container::create_base() {
    m_plot_windows.emplace_back("Plot Window Test");
    m_canvas_windows.emplace_back("Canvas Window Test");
}

}
