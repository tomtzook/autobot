#pragma once

#include <vector>

#include "obsr/window.h"
#include "plots/plots.h"

namespace ui {

class container {
public:
    container();

    void update();
    void draw();

private:
    void create_plot_window();

    obsr_window m_obsr_window;
    std::vector<plots::plot_window> m_plot_windows;
};

}
