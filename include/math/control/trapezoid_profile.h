#pragma once

#include "units.h"
#include "dashboard/object.h"

namespace autobot::math {

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
class trapezoid_profile {
public:
    using unit = units_actual(unit_);
    using velocity_unit = units_per_second(unit);
    using acceleration_unit = units_per_second(velocity_unit);
    using type = units::measure<unit>;
    using velocity_type = units::measure<velocity_unit>;
    using acceleration_type = units::measure<acceleration_unit>;

    struct state {
        type position;
        velocity_type velocity;
        bool finished;
    };

    trapezoid_profile(const velocity_type& max_velocity, const acceleration_type& max_acceleration);

    state calculate(const type& target_position, units::seconds current_time);

    void bind_dashboard(dashboard::bind&& bind);

private:
    velocity_type m_max_vel;
    acceleration_type m_max_accel;
    state m_last_output;

    dashboard::bind m_dashboard_bind;
};

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
trapezoid_profile<unit_>::trapezoid_profile(const velocity_type& max_velocity, const acceleration_type& max_acceleration)
    : m_max_vel(max_velocity) , m_max_accel(max_acceleration)
{}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
trapezoid_profile<unit_>::state trapezoid_profile<unit_>::calculate(const type& target_position, const units::seconds current_time) {
    const auto acceleration_time = m_max_vel / m_max_accel;
    const auto distance_passed_in_accel = m_max_accel * acceleration_time * acceleration_time * 0.5;
    const auto distance_passed_in_cruise = target_position - 2 * distance_passed_in_accel;
    const auto cruise_time = distance_passed_in_cruise / m_max_vel;

    state out;
    if (current_time <= acceleration_time) {
        // acceleration phase
        const auto time_in_phase = current_time;
        out = {
            m_max_accel * time_in_phase * time_in_phase * 0.5,
            m_max_accel * time_in_phase,
            false
        };
    } else if (current_time <= (acceleration_time + cruise_time)) {
        // cruising phase
        const auto time_in_phase = current_time - acceleration_time;
        out = {
            distance_passed_in_accel + m_max_vel * time_in_phase,
            m_max_vel,
            false
        };
    } else if (current_time <= (acceleration_time + acceleration_time + cruise_time)) {
        // deceleration phase
        const auto time_in_phase = current_time - acceleration_time - cruise_time;
        out = {
            (distance_passed_in_accel + distance_passed_in_cruise) + (m_max_vel * time_in_phase) - (m_max_accel * time_in_phase * time_in_phase * 0.5),
            m_max_vel - m_max_accel * time_in_phase,
            false
        };
    } else {
        out = {
            target_position,
            velocity_type(0),
            true
        };
    }

    m_last_output = out;
    return out;
}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
void trapezoid_profile<unit_>::bind_dashboard(dashboard::bind&& bind) {
    m_dashboard_bind = std::move(bind);

    m_dashboard_bind.set_type("trapezoid_profile");
    m_dashboard_bind.set_units(units::name<unit>());

    m_dashboard_bind.add("max_velocity", m_max_vel);
    m_dashboard_bind.add("max_acceleration", m_max_accel);

    std::function<type()> get_pos = [this]()->type { return m_last_output.position; };
    std::function<velocity_type()> get_vel = [this]()->velocity_type { return m_last_output.velocity; };
    std::function<bool()> get_finished = [this]()->bool { return m_last_output.finished; };

    m_dashboard_bind.add_func("next_position", std::move(get_pos), {});
    m_dashboard_bind.add_func("next_velocity", std::move(get_vel), {});
    m_dashboard_bind.add_func("finished", std::move(get_finished), {});
}

}
