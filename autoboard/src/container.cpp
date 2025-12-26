
#include "container.h"

namespace ui {

container::container()
    : m_obsr_window()
    , m_plot_windows() {
    create_plot_window();
}

void container::update() {
    m_obsr_window.update();

    for (auto& win : m_plot_windows) {
        win.update();
    }
}

void container::draw() {
    m_obsr_window.draw();

    for (auto& win : m_plot_windows) {
        win.draw();
    }
}

void container::create_plot_window() {
    m_plot_windows.emplace_back("Plot Window Test");
}

}
