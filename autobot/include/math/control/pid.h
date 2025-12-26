#pragma once

#include "math/common.h"
#include "units.h"
#include "math/ops.h"
#include "dashboard/object.h"

namespace autobot::math {

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
class pid_controller {
public:
    using error_unit = units_actual(in_unit_);
    using error_velocity_unit = units_per_second(error_unit);
    using out_unit = units_actual(out_unit_);

    using p_gain_unit = units_per(out_unit, error_unit);
    using i_gain_unit = units_per(out_unit, error_unit);
    using d_gain_unit = units_per(out_unit, error_velocity_unit);

    using raw_type = error_unit::type;
    using error_type = units::measure<error_unit>;
    using error_velocity_type = units::measure<error_velocity_unit>;
    using out_type = units::measure<out_unit>;
    using p_gain_type = units::measure<p_gain_unit>;
    using i_gain_type = units::measure<i_gain_unit>;
    using d_gain_type = units::measure<d_gain_unit>;

    static constexpr auto izone_0 = error_type(0);

    pid_controller();
    pid_controller(const pid_controller&) = default;
    pid_controller(pid_controller&&) = default;
    pid_controller(p_gain_type kp, i_gain_type ki, d_gain_type kd);
    pid_controller(raw_type kp, raw_type ki, raw_type kd);

    pid_controller& operator=(const pid_controller&) = default;
    pid_controller& operator=(pid_controller&&) = default;

    [[nodiscard]] p_gain_type kp() const;
    void kp(p_gain_type kp);
    void kp(raw_type kp);
    [[nodiscard]] i_gain_type ki() const;
    void ki(i_gain_type ki);
    void ki(raw_type ki);
    [[nodiscard]] d_gain_type kd() const;
    void kd(d_gain_type kd);
    void kd(raw_type kd);
    [[nodiscard]] error_type izone() const;
    void izone(error_type izone);
    [[nodiscard]] out_type min_out() const;
    void min_out(out_type min_out);
    [[nodiscard]] out_type max_out() const;
    void max_out(out_type max_out);

    void reset();
    [[nodiscard]] out_type calculate(const error_type& process_variable, const error_type& set_point, units::seconds dt);
    [[nodiscard]] bool at_setpoint(error_type position_tolerance, error_velocity_type velocity_tolerance) const;

    void bind_dashboard(dashboard::bind&& bind);

private:
    p_gain_type m_kp;
    i_gain_type m_ki;
    d_gain_type m_kd;
    error_type m_izone;
    out_type m_min_out;
    out_type m_max_out;

    bool m_is_first_run;
    error_type m_last_error;
    error_velocity_type m_last_velocity_error;
    error_type m_total_error;

    dashboard::bind m_dashboard_bind;
};

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
pid_controller<in_unit_, out_unit_>::pid_controller()
    : pid_controller(0, 0, 0)
{}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
pid_controller<in_unit_, out_unit_>::pid_controller(const p_gain_type kp, const i_gain_type ki, const d_gain_type kd)
    : m_kp(kp)
    , m_ki(ki)
    , m_kd(kd)
    , m_izone(0)
    , m_min_out(0)
    , m_max_out(0)
    , m_is_first_run(true)
    , m_last_error(0)
    , m_last_velocity_error(0)
    , m_total_error(0)
{}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
pid_controller<in_unit_, out_unit_>::pid_controller(const raw_type kp, const raw_type ki, const raw_type kd)
    : pid_controller(p_gain_type(kp), i_gain_type(ki), d_gain_type(kd))
{}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
pid_controller<in_unit_, out_unit_>::p_gain_type pid_controller<in_unit_, out_unit_>::kp() const {
    return m_kp;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::kp(const p_gain_type kp) {
    m_kp = kp;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::kp(const raw_type kp) {
    m_kp = kp;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
pid_controller<in_unit_, out_unit_>::i_gain_type pid_controller<in_unit_, out_unit_>::ki() const {
    return m_ki;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::ki(const i_gain_type ki) {
    m_ki = ki;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::ki(const raw_type ki) {
    m_ki = ki;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
pid_controller<in_unit_, out_unit_>::d_gain_type pid_controller<in_unit_, out_unit_>::kd() const {
    return m_kd;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::kd(const d_gain_type kd) {
    m_kd = kd;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::kd(const raw_type kd) {
    m_kd = kd;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
pid_controller<in_unit_, out_unit_>::error_type pid_controller<in_unit_, out_unit_>::izone() const {
    return m_izone;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::izone(const error_type izone) {
    m_izone = izone;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
pid_controller<in_unit_, out_unit_>::out_type pid_controller<in_unit_, out_unit_>::min_out() const {
    return m_min_out;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::min_out(const out_type min_out) {
    m_min_out = min_out;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
pid_controller<in_unit_, out_unit_>::out_type pid_controller<in_unit_, out_unit_>::max_out() const {
    return m_max_out;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::max_out(const out_type max_out) {
    m_max_out = max_out;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::reset() {
    m_is_first_run = true;
    m_last_error = 0;
    m_last_velocity_error = 0;
    m_total_error = 0;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
pid_controller<in_unit_, out_unit_>::out_type pid_controller<in_unit_, out_unit_>::calculate(const error_type& process_variable, const error_type& set_point, const units::seconds dt) {
    const auto error = set_point - process_variable;

    if (m_is_first_run) {
        m_last_error = error;
        m_is_first_run = false;
    }

    const auto velocity_error = (error - m_last_error) / dt;

    const auto p = m_kp * error;
    const auto i = m_ki * m_total_error;
    const auto d = m_kd * velocity_error;

    if (m_izone == izone_0 || abs(m_total_error) >= m_izone) {
        m_total_error = 0;
    } else {
        m_total_error += error;
    }

    m_last_error = error;
    m_last_velocity_error = velocity_error;

    const auto output = p + i + d;
    return constrain(output, m_min_out, m_max_out);
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
bool pid_controller<in_unit_, out_unit_>::at_setpoint(const error_type position_tolerance, const error_velocity_type velocity_tolerance) const {
    if (m_is_first_run) {
        return false;
    }

    return abs(m_last_error) <= position_tolerance && abs(m_last_velocity_error) <= velocity_tolerance;
}

template<units::unit_or_measure_type in_unit_, units::unit_or_measure_type out_unit_>
void pid_controller<in_unit_, out_unit_>::bind_dashboard(dashboard::bind&& bind) {
    m_dashboard_bind = std::move(bind);

    m_dashboard_bind.set_type("pid_controller");
    m_dashboard_bind.set_units(units::name<error_unit>());

    m_dashboard_bind.add("kp", m_kp);
    m_dashboard_bind.add("ki", m_ki);
    m_dashboard_bind.add("kd", m_kd);
    m_dashboard_bind.add("izone", m_izone);
    m_dashboard_bind.add("min_out", m_min_out);
    m_dashboard_bind.add("max_out", m_max_out);
    m_dashboard_bind.add_readonly("error", m_last_error);
    m_dashboard_bind.add_readonly("velocity_error", m_last_velocity_error);
    m_dashboard_bind.add_readonly("total_error", m_total_error);
}

}
