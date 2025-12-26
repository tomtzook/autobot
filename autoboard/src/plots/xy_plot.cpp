
#include "xy_plot.h"

#include <ranges>

namespace ui::plots {

xy_plot_data::xy_plot_data()
    : x_data{}
    , y_data{}
    , current_index(-1)
{}

void xy_plot_data::add(const float x, const float y) {
    current_index++;
    if (current_index >= measurement_count) {
        current_index = 1;
    }

    x_data[current_index] = x;
    y_data[current_index] = y;
}

void xy_plot_data::show_plot_line(const char* label, const float weight) const {
    ImPlot::SetNextLineStyle(ImVec4(0, 0, 0, -1), weight);
    ImPlot::PlotLine(label, x_data, y_data, current_index + 1);
}

xy_plot::xy_plot(const std::string_view label)
    : m_data()
    , m_label(label)
    , m_x_label("")
    , m_y_label("")
    , m_x_min(0)
    , m_x_max(1)
    , m_y_min(0)
    , m_y_max(1)
    , m_next_plot_id(0)
{}

const char* xy_plot::name() const {
    return m_label.c_str();
}

void xy_plot::setup_x_axis(const char* label, const float min_initial, const float max_initial) {
    m_x_label = label;
    m_x_min = min_initial;
    m_x_max = max_initial;
}

void xy_plot::setup_y_axis(const char* label, const float min_initial, const float max_initial) {
    m_y_label = label;
    m_y_min = min_initial;
    m_y_max = max_initial;
}

int xy_plot::create_plot(const std::string_view label, const float line_weight) {
    const auto id = m_next_plot_id++;
    m_data.emplace(id, sub_plot(label, line_weight));
    return id;
}

void xy_plot::add_data(const int plot_id, const float x, const float y) {
    if (const auto it = m_data.find(plot_id); it != m_data.end()) {
        it->second.add(x, y);
    }
}

void xy_plot::update(const float x) {
    if (x >= m_x_max) {
        const auto step = x - m_x_max;
        m_x_min += step;
        m_x_max += step;
    }
}

void xy_plot::draw() const {
    if (ImPlot::BeginPlot(m_label.c_str())) {
        ImPlot::SetupAxes(m_x_label, m_y_label);
        ImPlot::SetupAxisLimits(ImAxis_Y1, m_y_min, m_y_max, ImPlotCond_Once);
        ImPlot::SetupAxisLimits(ImAxis_X1, m_x_min, m_x_max, ImPlotCond_Always);

        for (const auto& plot: m_data | std::views::values) {
            plot.draw();
        }

        ImPlot::EndPlot();
    }
}

xy_plot::sub_plot::sub_plot()
    : m_label()
    , m_line_weight(0)
    , m_data()
{}

xy_plot::sub_plot::sub_plot(const std::string_view label, const float line_weight)
    : m_label(label)
    , m_line_weight(line_weight)
    , m_data()
{}

void xy_plot::sub_plot::add(const float x, const float y) {
    m_data.add(x, y);
}

void xy_plot::sub_plot::draw() const {
    m_data.show_plot_line(m_label.data(), m_line_weight);
}

}
