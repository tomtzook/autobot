#pragma once

#include <hal.h>
#include <hal_sim.h>
#include <units.h>

#include "dynamics/body.h"

namespace autobot::sim {

template<dynamics::joint_type joint_t_>
class item {
public:
    explicit item(dynamics::ligament<joint_t_>&& ligament)
        : m_ligament(std::move(ligament)) {}

    const dynamics::ligament<joint_t_>& ligament() const { return m_ligament; }
    dynamics::ligament<joint_t_>& ligament() { return m_ligament; }

protected:
    dynamics::ligament<joint_t_> m_ligament;
};

template<dynamics::joint_type joint_t_>
class hcsr04 : public item<joint_t_> {
public:
    static constexpr auto shape = dynamics::box_shape(units::meters(0.15), units::meters(0.45), units::meters(0.2));
    static constexpr auto min_range = units::convert<units::meters>(units::centimeters(2));
    static constexpr auto max_range = units::meters(4);
    static constexpr auto internal_timeout = units::milliseconds(38);
    static constexpr auto distance_centimeters_to_microsecond = 58;

    hcsr04(dynamics::ligament<joint_t_>&& ligament, const hal::device_id trig_id, const hal::device_id echo_id)
        : item<joint_t_>(std::move(ligament))
        , m_trig_id(trig_id)
        , m_echo_id(echo_id)
    {}

    void measure() {
        auto [origin, direction] = get_raycast_config();

        if (const auto result_opt = this->m_ligament.raycast(origin, direction, min_range, max_range); result_opt) {
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
        auto axis = this->m_ligament.forward();
        auto pos = this->m_ligament.get_world_position();
        auto origin = pos + shape.length * axis;
        return { origin, axis };
    }

    hal::device_id m_trig_id;
    hal::device_id m_echo_id;
};

}