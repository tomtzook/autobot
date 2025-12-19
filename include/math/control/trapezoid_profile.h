#pragma once

#include "units.h"

namespace autobot::math {

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
class trapezoid_profile {
public:
    using unit = units::underlying_unit<unit_>::type;
    using velocity_unit = units::compound_unit<unit, units::unit_inverse<units::units::seconds>>;
    using acceleration_unit = units::compound_unit<velocity_unit, units::unit_inverse<units::units::seconds>>;
    using type = units::measure<unit>;
    using velocity_type = units::measure<velocity_unit>;
    using acceleration_type = units::measure<acceleration_unit>;

    struct state {
        type position;
        velocity_type velocity;
    };

    trapezoid_profile(const velocity_type& max_velocity, const acceleration_type& max_acceleration);

    state calculate(const type& target_position, units::seconds current_time);

private:
    velocity_type m_max_vel;
    acceleration_type m_max_accel;
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
    const auto cruise_time = distance_passed_in_accel / m_max_vel;

    if (current_time <= acceleration_time) {
        // acceleration phase
        const auto time_in_phase = current_time;
        return {
            m_max_accel * time_in_phase * time_in_phase * 0.5,
            m_max_accel * time_in_phase
        };
    }

    if (current_time <= (acceleration_time + cruise_time)) {
        // cruising phase
        const auto time_in_phase = current_time - acceleration_time;
        return {
            distance_passed_in_accel + m_max_vel * time_in_phase,
            m_max_vel
        };
    }

    if (current_time <= (acceleration_time + acceleration_time + cruise_time)) {
        // deceleration phase
        const auto time_in_phase = current_time - acceleration_time - cruise_time;
        return {
            (distance_passed_in_accel + distance_passed_in_cruise) + (m_max_vel * time_in_phase) - (m_max_accel * time_in_phase * time_in_phase * 0.5),
            m_max_vel - m_max_accel * time_in_phase
        };
    }

    return {
        target_position,
        velocity_type(0)
    };
}

}
