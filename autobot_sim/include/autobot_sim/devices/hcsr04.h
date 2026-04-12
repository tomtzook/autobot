#pragma once

#include <hal_sim.h>
#include <autobot/units.h>

#include "autobot_sim/dynamics/body.h"

namespace autobot::sim::devices {

class hcsr04 : public dynamics::ligament {
public:
    static constexpr auto shape = dynamics::box_shape(units::meters(0.15), units::meters(0.45), units::meters(0.2));
    static constexpr auto min_range = units::convert<units::meters>(units::centimeters(2));
    static constexpr auto max_range = units::meters(4);
    static constexpr auto internal_timeout = units::milliseconds(38);
    static constexpr auto minimum_trigger_pulse = units::microseconds(6);
    static constexpr auto distance_centimeters_to_microsecond = 58;

    hcsr04(hal::device_id trig_id, hal::device_id echo_id);

    hcsr04(const hcsr04&) = delete;
    hcsr04(hcsr04&&) = delete;
    hcsr04& operator=(const hcsr04&) = delete;
    hcsr04& operator=(hcsr04&&) = delete;

    void measure();

private:
    std::pair<math::translation3, math::axis3> get_raycast_config();
    std::optional<units::microseconds> cast();

    hal::device_id m_trig_id;
    hal::device_id m_echo_id;
};

}