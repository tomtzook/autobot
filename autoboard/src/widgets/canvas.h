#pragma once

#include <variant>
#include <vector>
#include <imgui.h>

#include "data/source.h"

namespace ui::widgets {

class canvas_line;
class canvas_rect;
class canvas_circle;
class canvas_group;
class canvas_shape_wrapper;
using canvas_shape_type = std::optional<std::variant<canvas_line, canvas_rect, canvas_circle, canvas_group>>;

class canvas_line {
public:
    explicit canvas_line(data::data_source source);

    void update();
    void draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const;

private:
    data::data_source m_source;
    struct {
        ImVec2 p1;
        ImVec2 p2;
        ImU32 color;
    } m_state;
};

class canvas_rect {
public:
    explicit canvas_rect(data::data_source source);

    void update();
    void draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const;

private:
    data::data_source m_source;
    struct {
        ImVec2 p1;
        ImVec2 p2;
        ImU32 color;
    } m_state;
};

class canvas_circle {
public:
    explicit canvas_circle(data::data_source source);

    void update();
    void draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const;

private:
    data::data_source m_source;
    struct {
        ImVec2 p1;
        float radius;
        ImU32 color;
    } m_state;
};

class canvas_group {
public:
    explicit canvas_group(data::data_source source);

    void update();
    void draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const;

private:
    data::data_source m_source;
    struct {
        ImVec2 p;
    } m_state;

    std::map<uint64_t, canvas_shape_wrapper, std::less<>> m_shapes;
};

class canvas_shape_wrapper {
public:
    explicit canvas_shape_wrapper(data::data_source source);

    void update();
    void draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const;

private:
    data::data_source m_source;
    canvas_shape_type m_shape;
};

class canvas {
public:
    canvas() = default;

    void attach_data(data::data_source&& source);
    void update();
    void draw() const;

private:
    std::vector<canvas_shape_wrapper> m_shapes;
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
