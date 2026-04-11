#pragma once

#include <hal_sim.h>
#include <units.h>

#include "dynamics/body.h"

namespace autobot::sim {

class hcsr04 {
public:
    static constexpr auto shape = dynamics::box_shape(units::meters(0.15), units::meters(0.45), units::meters(0.2));
    static constexpr auto min_range = units::convert<units::meters>(units::centimeters(2));
    static constexpr auto max_range = units::meters(4);
    static constexpr auto internal_timeout = units::milliseconds(38);
    static constexpr auto distance_centimeters_to_microsecond = 58;

    hcsr04(const dynamics::body_node& body_node, const hal::device_id trig_id, const hal::device_id echo_id)
        : m_body_node(body_node)
        , m_trig_id(trig_id)
        , m_echo_id(echo_id)
    {}

    void measure() {
        auto [origin, direction] = get_raycast_config();

        if (const auto result_opt = m_body_node.raycast(origin, direction, min_range, max_range); result_opt) {
            const auto& result = result_opt.value();
            const auto pulse_length = units::microseconds(units::convert<units::centimeters>(result.distance).value() * distance_centimeters_to_microsecond);
            hal::sim::set_value_u32(m_echo_id, hal::value_pulsewidth_length, pulse_length.value());
        } else {
            constexpr auto pulse_length = units::convert<units::microseconds>(internal_timeout);
            hal::sim::set_value_u32(m_echo_id, hal::value_pulsewidth_length, pulse_length.value());
        }
    }

private:
    std::pair<Eigen::Vector3d, Eigen::Vector3d> get_raycast_config() {
        auto axis = m_body_node.forward();
        auto pos = m_body_node.get_world_position();
        auto origin = pos + shape.length.value() * axis;
        return { origin, axis };
    }

    dynamics::body_node m_body_node;
    hal::device_id m_trig_id;
    hal::device_id m_echo_id;
};

}