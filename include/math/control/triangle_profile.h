#pragma once

#include "units.h"

namespace autobot::math {

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
class triangle_profile {
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
    };

    triangle_profile(const velocity_type& max_velocity, const acceleration_type& max_acceleration);

    state calculate(const type& target_position, units::seconds current_time);

private:
    velocity_type m_max_vel;
    acceleration_type m_max_accel;
};

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
triangle_profile<unit_>::triangle_profile(const velocity_type& max_velocity, const acceleration_type& max_acceleration)
    : m_max_vel(max_velocity) , m_max_accel(max_acceleration)
{}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
triangle_profile<unit_>::state triangle_profile<unit_>::calculate(const type& target_position, const units::seconds current_time) {
    // todo: implement
    return {
        target_position,
        velocity_type(0)
    };
}

}
