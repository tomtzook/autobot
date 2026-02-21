#pragma once

#include <vector>

#include "widgets/obsr.h"
#include "plots/plots.h"
#include "widgets/canvas.h"
#include "render/render.h"
#include "render/camera.h"
#include "render/body.h"

namespace ui {

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
    render::renderer m_renderer3d;
    render::camera m_camera;
    render::world3d m_world3d;
};

}
