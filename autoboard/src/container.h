#pragma once

#include <vector>

#include <glui/render.h>
#include <glui/camera.h>

#include "widgets/obsr.h"
#include "plots/plots.h"
#include "widgets/canvas.h"
#include "render/body.h"

namespace autobot::board::ui {

class container {
public:
    container();

    void update(GLFWwindow* window);
    void draw_ui();
    void render3d();

private:
    void create_base();

    data::obsr_storage m_obsr_storage;
    widgets::obsr_tree m_obsr_tree;

    std::vector<plots::plot_window> m_plot_windows;
    std::vector<widgets::canvas_window> m_canvas_windows;
    glui::renderer m_renderer3d;
    glui::camera m_camera;
    render::world3d m_world3d;
};

}
