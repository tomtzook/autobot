
#include "container.h"

namespace ui {

container::container()
    : m_obsr_storage()
    , m_obsr_tree(m_obsr_storage)
    , m_plot_windows()
    , m_canvas_windows() {
    create_base();
}

void container::update() {
    for (auto& win : m_plot_windows) {
        win.update();
    }
    for (auto& win : m_canvas_windows) {
        win.update();
    }
}

void container::draw() {
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

void container::create_base() {
    m_plot_windows.emplace_back("Plot Window Test");
    m_canvas_windows.emplace_back("Canvas Window Test");
}

}
