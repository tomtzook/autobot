
#include <format>

#include "data/registry.h"
#include "plots.h"

namespace ui::plots {

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

void time_plot::attach_data(data::data_source&& source) {
    const auto id = m_plot.create_plot(source.get_name(), 3.0f);
    m_sources.emplace_back(id, std::move(source));
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
            if (source.data_source.get_scheme() != data::scheme::type::number) {
                continue;
            }

            const auto data = source.data_source.read<data::scheme::number>();
            m_plot.add_data(source.plot_id, seconds, data.value);
        }

        m_plot.update(seconds);
    }
}

void time_plot::draw() {
    m_plot.draw();
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

            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DATA_SOURCE")) {
                    const auto item_id = *static_cast<uint64_t*>(payload->Data);
                    const auto& registry = data::get_registry();
                    if (auto opt = registry.get(item_id)) {
                        auto& data = opt.value();
                        it->attach_data(std::move(data));
                    }
                }
                ImGui::EndDragDropTarget();
            }

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
