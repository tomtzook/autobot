#pragma once

#include <chrono>

namespace ui::plots {

template<typename t_>
concept updatable = requires(t_ t, float f1_x) {
    {t.update(f1_x)} -> std::same_as<void>;
};

template<updatable t_>
class timed_plot_updater {
public:
    using plot_type = t_;

    timed_plot_updater(plot_type* plot, std::chrono::milliseconds period);

    void update();

private:
    plot_type* m_plot;

    std::chrono::milliseconds m_period;
    bool m_first_run;
    std::chrono::milliseconds m_start_time;
    std::chrono::milliseconds m_last_update;
};

template<updatable t_>
timed_plot_updater<t_>::timed_plot_updater(plot_type* plot, const std::chrono::milliseconds period)
    : m_plot(plot)
    , m_period(period)
    , m_first_run(true)
    , m_start_time()
    , m_last_update()
{}

template<updatable t_>
void timed_plot_updater<t_>::update() {
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

        m_plot->update(seconds);
    }
}

}
