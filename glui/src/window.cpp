
#include <cstdio>
#include <glad/glad.h>

#include "glui/window.h"

#include <unistd.h>


namespace glui {

static void glfw_error_callback(const int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static GLFWwindow* create_window(const std::string_view name, const uint16_t width, const uint16_t height) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        // todo: throw
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
    if (window == nullptr) {
        // todo: throw
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        // todo: throw
        return nullptr;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    return window;
}

window::window(const std::string_view name, const uint16_t width, const uint16_t height, glm::vec4&& clear_color)
    : m_window(create_window(name, width, height))
    , m_clear_color(clear_color)
    , m_last_time(glfwGetTime())
    , m_update_actions()
    , m_render_actions()
{}

window::~window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool window::should_close() {
    return glfwWindowShouldClose(m_window);
}

bool window::is_iconified() {
    return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0;
}

int window::get_key(const int key) {
    return glfwGetKey(m_window, key);
}

void window::on_update(update_action&& action, double dt) {
    m_update_actions.emplace_back(std::move(action), dt);
}

void window::on_render(render_action&& action) {
    m_render_actions.emplace_back(action);
}

bool window::iterate() {
    if (should_close()) {
        return false;
    }

    glfwPollEvents();
    if (is_iconified()) {
        sleep(10);
        return true;
    }

    const auto time = glfwGetTime();
    const auto frame_time = time - m_last_time;
    m_last_time = time;

    for (auto& context : m_update_actions) {
        context.accumulated_time += frame_time;
    }

    bool updates_required = false;
    do {
        bool did_update = false;
        for (auto& context : m_update_actions) {
            if (context.accumulated_time >= context.dt) {
                context.accumulated_time -= context.dt;
                context.action();
                did_update = true;
            }
        }

        updates_required = did_update;
    } while (updates_required);

    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(m_clear_color.x * m_clear_color.w, m_clear_color.y * m_clear_color.w, m_clear_color.z * m_clear_color.w, m_clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& action : m_render_actions) {
        action();
    }

    glfwSwapBuffers(m_window);

    return true;
}

window::update_context::update_context(update_action&& action, const double dt)
    : action(action)
    , dt(dt)
    , accumulated_time(0)
{}

}
