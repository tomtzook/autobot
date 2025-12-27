
#include <cmath>

#include "data/registry.h"
#include "canvas.h"

namespace ui::widgets {

// from pixels
static constexpr auto scale_factor = 10.0f;

static ImVec2 translate_pos_to_screen(const ImVec2& pos, const ImVec2& min, const ImVec2& max) {
    return ImVec2(min[0] + pos[0] * scale_factor, max[1] - pos[1] * scale_factor);
}

static float translate_angle(float angle) {
    static constexpr auto circle = M_PIf * 2;
    angle = std::fmod(angle, circle);
    if (angle < circle) {
        angle += circle;
    }

    return angle;
}

canvas_line::canvas_line(data::data_source&& source)
    : m_source(source)
    , m_state()
{}

void canvas_line::update() {
    if (m_source.get_scheme() != data::scheme::canvas_line) {
        return;
    }

    const auto data = m_source.read<data::canvas_line_scheme>();
    const auto angle = translate_angle(data.angle);
    m_state.p1.x = data.x;
    m_state.p1.y = data.y;
    m_state.p2.x = (m_state.p1.x + data.length * std::cos(angle));
    m_state.p2.y = (m_state.p1.y + data.length * std::sin(angle));
}

void canvas_line::draw(ImDrawList* draw_list, const ImVec2& min, const ImVec2& max) const {
    const auto p1 = translate_pos_to_screen(m_state.p1, min, max);
    const auto p2 = translate_pos_to_screen(m_state.p2, min, max);
    draw_list->AddLine(p1, p2, IM_COL32_WHITE, 5.0f);
}

void canvas::attach_data(data::data_source&& source) {
    switch (source.get_scheme()) {
        case data::scheme::canvas_line: {
            m_shapes.emplace_back(canvas_line{std::move(source)});
            break;
        }
        default:
            break;
    }
}

void canvas::update() {
    for (auto& shape : m_shapes) {
        std::visit([](auto& value) {
            value.update();
        }, shape);
    }
}

void canvas::draw() {
    const auto win_pos = ImGui::GetWindowPos();
    const auto cursor_pos = ImGui::GetCursorPos();
    const auto pos = ImVec2(win_pos[0] + cursor_pos[0], win_pos[1] + cursor_pos[1]);

    auto* draw_list = ImGui::GetWindowDrawList();

    const auto size = ImGui::GetContentRegionAvail();
    const auto canvas_min = pos;
    const auto canvas_max = ImVec2(pos[0] + size[0], pos[1] + size[1]);

    draw_list->AddRectFilled(canvas_min, canvas_max, IM_COL32_BLACK);
    for (auto& shape : m_shapes) {
        std::visit([&draw_list, &canvas_min, &canvas_max](auto& value) {
            value.draw(draw_list, canvas_min, canvas_max);
        }, shape);
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
