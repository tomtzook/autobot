#pragma once

#include <variant>
#include <vector>
#include <imgui.h>

#include "data/source.h"

namespace ui::widgets {

class canvas_line {
public:
    explicit canvas_line(data::data_source&& source);

    void update();
    void draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const;

private:
    data::data_source m_source;
    struct {
        ImVec2 p1;
        ImVec2 p2;
    } m_state;
};

class canvas {
public:
    canvas() = default;

    void attach_data(data::data_source&& source);
    void update();
    void draw();

private:
    using shape_type = std::variant<canvas_line>;
    std::vector<shape_type> m_shapes;
};

class canvas_window {
public:
    explicit canvas_window(std::string_view label);

    void update();
    void draw();

private:
    std::string m_label;
    canvas m_canvas;
};

}
