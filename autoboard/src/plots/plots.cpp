
#include "plots.h"

#include <format>

#include "plots.h"

namespace ui::plots {

static float get_value(const obsr::handle handle) {
    switch (const auto value = obsr::get_value(handle); value.get_type()) {
        case obsr::value_type::boolean: return value.get_boolean();
        case obsr::value_type::integer32: return static_cast<float>(value.get_int32());
        case obsr::value_type::integer64: return static_cast<float>(value.get_int64());
        case obsr::value_type::floating_point32: return value.get_float();
        case obsr::value_type::floating_point64: return static_cast<float>(value.get_double());
        default: return 0.0f;
    }
}

static std::string get_item_name(const obsr::handle handle) {
    return obsr::get_path_for_entry(handle);
}

time_plot::time_plot(const std::string_view label)
    : m_plot(label)
    , m_period(update_period)
    , m_first_run(true)
    , m_start_time()
    , m_last_update() {
    m_plot.setup_x_axis("Time [Seconds]", 0, 10);
    m_plot.setup_y_axis("Y", 0, 1);
}

const char* time_plot::name() const {
    return m_plot.name();
}

void time_plot::update() {
    const auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    if (m_first_run) {
        m_first_run = false;
        m_start_time = now;
        m_last_update = now;
    }

    if ((now - m_last_update) >= m_period) {
        m_last_update = now;
        const auto timestamp = now - m_start_time;
        const auto seconds = static_cast<float>(timestamp.count()) / 1000.0f;

        for (auto& source : m_sources) {
            const auto value = get_value(source.m_handle);
            m_plot.add_data(source.plot_id, seconds, value);
        }

        m_plot.update(seconds);
    }
}

void time_plot::draw() {
    m_plot.draw();

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBSR_ENTRY")) {
            obsr::handle item_handle = *static_cast<obsr::handle*>(payload->Data);
            const auto name = get_item_name(item_handle);
            const auto id = m_plot.create_plot(name, 1.0f);
            m_sources.emplace_back(id, item_handle);
        }
        ImGui::EndDragDropTarget();
    }
}

plot_window::plot_window(const std::string_view label)
    : m_label(label)
    , m_plots()
    , m_next_plot_index(0)
{}

const char* plot_window::name() const {
    return m_label.c_str();
}

void plot_window::update() {
    for (auto& plot : m_plots) {
        plot.update();
    }
}

void plot_window::draw() {
    ImGui::Begin(m_label.c_str());

    for (auto it = m_plots.begin(); it != m_plots.end();) {
        ImGui::PushID(it->name());

        if (ImGui::Button("Delete")) {
            it = m_plots.erase(it);
        } else {
            it->draw();
            ++it;
        }

        ImGui::PopID();
    }

    if (ImGui::Button("Add")) {
        const auto id = m_next_plot_index++;
        std::string name = std::format("Plot {}", id);

        m_plots.emplace_back(name);
    }

    ImGui::End();
}

}
