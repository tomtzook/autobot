#pragma once

#include "math/common.h"
#include "units.h"
#include "math/ops.h"

namespace autobot::math {

template<units::unit_or_measure_type unit_>
class pid_controller {
public:
    using unit = units_actual(unit_);
    using velocity_unit = units_per_second(unit);
    using raw_type = unit::type;
    using type = units::measure<unit>;
    using velocity_type = units::measure<velocity_unit>;

    static constexpr auto izone_0 = type(0);

    pid_controller();
    pid_controller(const pid_controller&) = default;
    pid_controller(pid_controller&&) = default;
    pid_controller(raw_type kp, raw_type ki, raw_type kd);

    pid_controller& operator=(const pid_controller&) = default;
    pid_controller& operator=(pid_controller&&) = default;

    [[nodiscard]] raw_type kp() const;
    void kp(raw_type kp);
    [[nodiscard]] raw_type ki() const;
    void ki(raw_type ki);
    [[nodiscard]] raw_type kd() const;
    void kd(raw_type kd);
    [[nodiscard]] type izone() const;
    void izone(type izone);
    [[nodiscard]] raw_type min_out() const;
    void min_out(raw_type min_out);
    [[nodiscard]] raw_type max_out() const;
    void max_out(raw_type max_out);

    void reset();
    [[nodiscard]] raw_type calculate(const type& process_variable, const type& set_point, units::seconds dt);
    [[nodiscard]] bool at_setpoint(type position_tolerance, velocity_type velocity_tolerance) const;

private:
    raw_type m_kp;
    raw_type m_ki;
    raw_type m_kd;
    type m_izone;
    raw_type m_min_out;
    raw_type m_max_out;

    bool m_is_first_run;
    type m_last_error;
    velocity_type m_last_velocity_error;
    type m_total_error;
};

template<units::unit_or_measure_type unit_>
pid_controller<unit_>::pid_controller()
    : pid_controller(0, 0, 0)
{}

template<units::unit_or_measure_type unit_>
pid_controller<unit_>::pid_controller(const raw_type kp, const raw_type ki, const raw_type kd)
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

template<units::unit_or_measure_type unit_>
pid_controller<unit_>::raw_type pid_controller<unit_>::kp() const {
    return m_kp;
}

template<units::unit_or_measure_type unit_>
void pid_controller<unit_>::kp(const raw_type kp) {
    m_kp = kp;
}

template<units::unit_or_measure_type unit_>
pid_controller<unit_>::raw_type pid_controller<unit_>::ki() const {
    return m_ki;
}

template<units::unit_or_measure_type unit_>
void pid_controller<unit_>::ki(const raw_type ki) {
    m_ki = ki;
}

template<units::unit_or_measure_type unit_>
pid_controller<unit_>::raw_type pid_controller<unit_>::kd() const {
    return m_kd;
}

template<units::unit_or_measure_type unit_>
void pid_controller<unit_>::kd(const raw_type kd) {
    m_kd = kd;
}

template<units::unit_or_measure_type unit_>
pid_controller<unit_>::type pid_controller<unit_>::izone() const {
    return m_izone;
}

template<units::unit_or_measure_type unit_>
void pid_controller<unit_>::izone(const type izone) {
    m_izone = izone;
}

template<units::unit_or_measure_type unit_>
pid_controller<unit_>::raw_type pid_controller<unit_>::min_out() const {
    return m_min_out;
}

template<units::unit_or_measure_type unit_>
void pid_controller<unit_>::min_out(const raw_type min_out) {
    m_min_out = min_out;
}

template<units::unit_or_measure_type unit_>
pid_controller<unit_>::raw_type pid_controller<unit_>::max_out() const {
    return m_max_out;
}

template<units::unit_or_measure_type unit_>
void pid_controller<unit_>::max_out(const raw_type max_out) {
    m_max_out = max_out;
}

template<units::unit_or_measure_type unit_>
void pid_controller<unit_>::reset() {
    m_is_first_run = true;
    m_last_error = 0;
    m_last_velocity_error = 0;
    m_total_error = 0;
}

template<units::unit_or_measure_type unit_>
pid_controller<unit_>::raw_type pid_controller<unit_>::calculate(const type& process_variable, const type& set_point, const units::seconds dt) {
    const auto error = set_point - process_variable;

    if (m_is_first_run) {
        m_last_error = error;
        m_is_first_run = false;
    }

    const auto velocity_error = (error - m_last_error) / dt;

    const auto p = m_kp * error.value();
    const auto i = m_ki * m_total_error.value();
    const auto d = m_kd * velocity_error.value();

    if (m_izone != izone_0 && abs(m_total_error) < m_izone) {
        m_total_error = 0;
    } else {
        m_total_error += error;
    }

    m_last_error = error;
    m_last_velocity_error = velocity_error;

    const auto output = p + i + d;
    return constrain(output, m_min_out, m_max_out);
}

template<units::unit_or_measure_type unit_>
bool pid_controller<unit_>::at_setpoint(const type position_tolerance, const velocity_type velocity_tolerance) const {
    return abs(m_last_error) <= position_tolerance && abs(m_last_velocity_error) <= velocity_tolerance;
}

}
