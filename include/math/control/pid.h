#pragma once

#include "math/common.h"

namespace autobot::math {

template<numeric type_ = floating_type>
class pid {
public:
    using type = type_;

    pid();
    pid(const pid&) = default;
    pid(pid&&) = default;
    pid(type kp, type ki, type kd);

    pid& operator=(const pid&) = default;
    pid& operator=(pid&&) = default;

    [[nodiscard]] type kp() const;
    void kp(type kp);
    [[nodiscard]] type ki() const;
    void ki(type ki);
    [[nodiscard]] type kd() const;
    void kd(type kd);
    [[nodiscard]] type izone() const;
    void izone(type izone);
    [[nodiscard]] type min_out() const;
    void min_out(type min_out);
    [[nodiscard]] type max_out() const;
    void max_out(type max_out);

    void reset();
    [[nodiscard]] type calculate(type process_variable, type set_point, type dt);
    [[nodiscard]] bool at_setpoint(type position_tolerance, type velocity_tolerance) const;

private:
    type m_kp;
    type m_ki;
    type m_kd;
    type m_izone;
    type m_min_out;
    type m_max_out;

    bool m_is_first_run;
    type m_last_error;
    type m_last_velocity_error;
    type m_total_error;
};

template<numeric type_>
pid<type_>::pid()
    : pid(0, 0, 0)
{}

template<numeric type_>
pid<type_>::pid(const type kp, const type ki, const type kd)
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

template<numeric type_>
pid<type_>::type pid<type_>::kp() const {
    return m_kp;
}

template<numeric type_>
void pid<type_>::kp(const type kp) {
    m_kp = kp;
}

template<numeric type_>
pid<type_>::type pid<type_>::ki() const {
    return m_ki;
}

template<numeric type_>
void pid<type_>::ki(const type ki) {
    m_ki = ki;
}

template<numeric type_>
pid<type_>::type pid<type_>::kd() const {
    return m_kd;
}

template<numeric type_>
void pid<type_>::kd(const type kd) {
    m_kd = kd;
}

template<numeric type_>
pid<type_>::type pid<type_>::izone() const {
    return m_izone;
}

template<numeric type_>
void pid<type_>::izone(const type izone) {
    m_izone = izone;
}

template<numeric type_>
pid<type_>::type pid<type_>::min_out() const {
    return m_min_out;
}

template<numeric type_>
void pid<type_>::min_out(const type min_out) {
    m_min_out = min_out;
}

template<numeric type_>
pid<type_>::type pid<type_>::max_out() const {
    return m_max_out;
}

template<numeric type_>
void pid<type_>::max_out(const type max_out) {
    m_max_out = max_out;
}

template<numeric type_>
void pid<type_>::reset() {
    m_is_first_run = true;
    m_last_error = 0;
    m_last_velocity_error = 0;
    m_total_error = 0;
}

template<numeric type_>
pid<type_>::type pid<type_>::calculate(const type process_variable, const type set_point, const type dt) {
    const auto error = set_point - process_variable;

    if (m_is_first_run) {
        m_last_error = error;
        m_is_first_run = false;
    }

    const auto velocity_error = (error - m_last_error) / dt;

    const auto p = m_kp * error;
    const auto i = m_ki * m_total_error;
    const auto d = m_kd * velocity_error;

    if (m_izone != 0 && abs(m_total_error) < m_izone) {
        m_total_error = 0;
    } else {
        m_total_error += error;
    }

    m_last_error = error;
    m_last_velocity_error = velocity_error;

    const auto output = p + i + d;
    return constrain(output, m_min_out, m_max_out);
}

template<numeric type_>
bool pid<type_>::at_setpoint(const type position_tolerance, const type velocity_tolerance) const {
    return abs(m_last_error) <= position_tolerance && abs(m_last_velocity_error) <= velocity_tolerance;
}

}
