#pragma once

#include <vector>

#include "widgets/obsr.h"
#include "plots/plots.h"
#include "widgets/canvas.h"

namespace ui {

class container {
public:
    container();

    void update();
    void draw();

private:
    void create_base();

    data::obsr_storage m_obsr_storage;
    widgets::obsr_tree m_obsr_tree;

    std::vector<plots::plot_window> m_plot_windows;
    std::vector<widgets::canvas_window> m_canvas_windows;
};

}
