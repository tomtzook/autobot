
#include <cmath>

#include "data/registry.h"
#include "canvas.h"

#include <ranges>

namespace ui::widgets {

// from pixels
static constexpr auto scale_factor = 10.0f;

static ImVec2 translate_pos_to_screen(const ImVec2& pos, const ImVec2& min, const ImVec2& max) {
    return {min[0] + pos[0] * scale_factor, max[1] - pos[1] * scale_factor};
}

static float translate_angle(float angle) {
    static constexpr auto circle = M_PIf * 2;
    angle = std::fmod(angle, circle);
    if (angle < circle) {
        angle += circle;
    }

    return angle;
}

static canvas_shape_type create_shape_by_type(data::data_source& source) {
    switch (source.get_scheme()) {
        case data::scheme::type::canvas_line:
            return canvas_line(source);
        case data::scheme::type::canvas_rect:
            return canvas_rect(source);
        case data::scheme::type::canvas_circle:
            return canvas_circle(source);
        case data::scheme::type::canvas_group:
            return canvas_group(source);
        default:
            return std::nullopt;
    }
}

static bool should_reload_shape(const canvas_shape_type& current_shape, const data::data_source& source) {
    switch (source.get_scheme()) {
        case data::scheme::type::canvas_line:
            return !current_shape.has_value() || !std::holds_alternative<canvas_line>(current_shape.value());
        case data::scheme::type::canvas_rect:
            return !current_shape.has_value() || !std::holds_alternative<canvas_rect>(current_shape.value());
        case data::scheme::type::canvas_circle:
            return !current_shape.has_value() || !std::holds_alternative<canvas_circle>(current_shape.value());
        case data::scheme::type::canvas_group:
            return !current_shape.has_value() || !std::holds_alternative<canvas_group>(current_shape.value());
        default:
            return false;
    }
}

canvas_line::canvas_line(data::data_source source)
    : m_source(std::move(source))
    , m_state()
{}

void canvas_line::update() {
    if (m_source.get_scheme() != data::scheme::type::canvas_line || !m_source.has_new_data()) {
        return;
    }

    const auto data = m_source.read<data::scheme::canvas_line>();
    const auto angle = translate_angle(data.angle);
    m_state.p1.x = data.x;
    m_state.p1.y = data.y;
    m_state.p2.x = (m_state.p1.x + data.length * std::cos(angle));
    m_state.p2.y = (m_state.p1.y + data.length * std::sin(angle));
    m_state.color = data.color;
}

void canvas_line::draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const {
    const auto p1 = translate_pos_to_screen(m_state.p1, min, max);
    const auto p2 = translate_pos_to_screen(m_state.p2, min, max);
    draw_list->AddLine(p1, p2, m_state.color, 5.0f);
}

canvas_rect::canvas_rect(data::data_source source)
    : m_source(std::move(source))
    , m_state()
{}

void canvas_rect::update() {
    if (m_source.get_scheme() != data::scheme::type::canvas_rect || !m_source.has_new_data()) {
        return;
    }

    const auto data = m_source.read<data::scheme::canvas_rect>();
    m_state.p1.x = data.x;
    m_state.p1.y = data.y;
    m_state.p2.x = (m_state.p1.x + data.width);
    m_state.p2.y = (m_state.p1.y + data.length);
    m_state.color = data.color;
}

void canvas_rect::draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const {
    const auto p1 = translate_pos_to_screen(m_state.p1, min, max);
    const auto p2 = translate_pos_to_screen(m_state.p2, min, max);
    draw_list->AddRect(p1, p2, m_state.color, 0.0f, 0, 5.0f);
}

canvas_circle::canvas_circle(data::data_source source)
    : m_source(std::move(source))
    , m_state()
{}

void canvas_circle::update() {
    if (m_source.get_scheme() != data::scheme::type::canvas_circle || !m_source.has_new_data()) {
        return;
    }

    const auto data = m_source.read<data::scheme::canvas_circle>();
    m_state.p1.x = data.x;
    m_state.p1.y = data.y;
    m_state.radius = data.radius;
    m_state.color = data.color;
}

void canvas_circle::draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const {
    const auto p1 = translate_pos_to_screen(m_state.p1, min, max);
    draw_list->AddCircle(p1, m_state.radius, m_state.color, 0, 5.0f);
}

canvas_group::canvas_group(data::data_source source)
    : m_source(std::move(source))
    , m_state()
    , m_shapes()
{}

void canvas_group::update() {
    if (m_source.get_scheme() != data::scheme::type::canvas_group || !m_source.has_new_data()) {
        return;
    }

    const auto data = m_source.read<data::scheme::canvas_group>();
    m_state.p.x = data.x;
    m_state.p.y = data.y;

    auto& registry = data::get_registry();
    registry.foreach_child(m_source.get_id(), [this, &registry](const data::data_source& child)->void {
        if (const auto it = m_shapes.find(child.get_id()); it == m_shapes.end() && registry.has_children(child.get_id())) {
            m_shapes.emplace(child.get_id(), canvas_shape_wrapper(child));
        }
    });

    for (auto& shape: m_shapes | std::views::values) {
        shape.update();
    }
}

void canvas_group::draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const {
    const auto new_min = translate_pos_to_screen(m_state.p, min, max);

    for (const auto& shape: m_shapes | std::views::values) {
        shape.draw(draw_list, new_min, max);
    }
}

canvas_shape_wrapper::canvas_shape_wrapper(data::data_source source)
    : m_source(std::move(source))
    , m_shape()
{}

void canvas_shape_wrapper::update() {
    if (should_reload_shape(m_shape, m_source)) {
        if (auto new_shape = create_shape_by_type(m_source)) {
            m_shape.emplace(std::move(new_shape.value()));
        } else {
            m_shape.reset();
        }
    }

    if (!m_shape) {
        return;
    }

    auto& shape = m_shape.value();
    std::visit([](auto& value) {
        value.update();
    }, shape);
}

void canvas_shape_wrapper::draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const {
    if (!m_shape) {
        return;
    }

    auto& shape = m_shape.value();
    std::visit([&draw_list, &min, &max](auto& value) {
        value.draw(draw_list, min, max);
    }, shape);
}

void canvas::attach_data(data::data_source&& source) {
    m_shapes.emplace_back(std::move(source));
}

void canvas::update() {
    for (auto& shape : m_shapes) {
        shape.update();
    }
}

void canvas::draw() const {
    const auto win_pos = ImGui::GetWindowPos();
    const auto cursor_pos = ImGui::GetCursorPos();
    const auto pos = ImVec2(win_pos[0] + cursor_pos[0], win_pos[1] + cursor_pos[1]);

    auto* draw_list = ImGui::GetWindowDrawList();

    const auto size = ImGui::GetContentRegionAvail();
    const auto canvas_min = pos;
    const auto canvas_max = ImVec2(pos[0] + size[0], pos[1] + size[1]);

    draw_list->AddRectFilled(canvas_min, canvas_max, IM_COL32_BLACK_TRANS);
    for (auto& shape : m_shapes) {
        shape.draw(draw_list, canvas_min, canvas_max);
    }

    ImGui::InvisibleButton("canvas", size);
}

canvas_window::canvas_window(const std::string_view label)
    : m_label(label)
    , m_canvas()
{}

void canvas_window::update() {
    m_canvas.update();
}

void canvas_window::draw() {
    ImGui::Begin(m_label.c_str());

    m_canvas.draw();
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DATA_SOURCE")) {
            const auto item_id = *static_cast<uint64_t*>(payload->Data);
            const auto& registry = data::get_registry();
            if (auto opt = registry.get(item_id)) {
                auto& data = opt.value();
                m_canvas.attach_data(std::move(data));
            }
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::End();
}

}
