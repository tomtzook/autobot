#pragma once

#include <chrono>

#include "data/source.h"
#include "plots/xy_plot.h"

namespace ui::plots {

class time_plot {
public:
    explicit time_plot(std::string_view label);

    [[nodiscard]] const char* name() const;

    void attach_data(data::data_source&& source);
    void update();
    void draw();

private:
    static constexpr auto update_period = std::chrono::milliseconds(100);
    struct source_node {
        int plot_id;
        data::data_source data_source;
    };

    xy_plot m_plot;
    std::vector<source_node> m_sources;

    std::chrono::milliseconds m_period;
    bool m_first_run;
    std::chrono::milliseconds m_start_time;
    std::chrono::milliseconds m_last_update;
};

class plot_window {
public:
    explicit plot_window(std::string_view label);

    [[nodiscard]] const char* name() const;

    void update();
    void draw();

private:
    std::string m_label;
    std::vector<time_plot> m_plots;

    int m_next_plot_index;
};

}
