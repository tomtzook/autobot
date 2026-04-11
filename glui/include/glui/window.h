#pragma once

#include <string_view>
#include <functional>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace glui {

class window {
public:
    using update_action = std::function<void()>;
    using render_action = std::function<void()>;

    window(std::string_view name, uint16_t width, uint16_t height, glm::vec4&& clear_color);
    ~window();

    [[nodiscard]] bool should_close();
    [[nodiscard]] bool is_iconified();
    [[nodiscard]] int get_key(int key);

    void on_update(update_action&& action, double dt);
    void on_render(render_action&& action);

    bool iterate();

private:
    struct update_context {
        update_context(update_action&& action, double dt);

        update_action action;
        double dt;
        double accumulated_time;
    };

    GLFWwindow* m_window;
    glm::vec4 m_clear_color;
    double m_last_time;
    std::vector<update_context> m_update_actions;
    std::vector<render_action> m_render_actions;
};

}
