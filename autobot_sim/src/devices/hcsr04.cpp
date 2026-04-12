
#include "autobot_sim/devices/hcsr04.h"

namespace autobot::sim::devices {

hcsr04::hcsr04(const hal::device_id trig_id, const hal::device_id echo_id)
    : m_trig_id(trig_id)
    , m_echo_id(echo_id) {
    hal::sim::set_value_callback(echo_id, hal::value_digital_io_signal, [this](hal::device_id, hal::value_key, const hal::sim::generic_value& value)->void {
        if (value.is_pulse && units::microseconds(value.pulse_info.duration) >= minimum_trigger_pulse) {
            measure();
        }
    });
}

void hcsr04::measure() {
    if (const auto cast_opt = cast(); cast_opt) {
        const auto pulse_length = static_cast<uint32_t>(cast_opt.value().value());
        hal::sim::set_value_u32(m_echo_id, hal::value_pulsewidth_length, pulse_length);
    } else {
        constexpr auto pulse_length = units::convert<units::microseconds>(internal_timeout);
        hal::sim::set_value_u32(m_echo_id, hal::value_pulsewidth_length, pulse_length.value());
    }
}

std::pair<math::translation3, math::axis3> hcsr04::get_raycast_config() {
    auto axis = forward();
    const auto pos = get_translation();
    auto origin = pos + axis * shape.length;
    return { origin, axis };
}

std::optional<units::microseconds> hcsr04::cast() {
    auto [origin, direction] = get_raycast_config();
    if (const auto result_opt = raycast(origin, direction, min_range, max_range); result_opt) {
        const auto& distance = result_opt.value();
        auto pulse_length = units::microseconds(units::convert<units::centimeters>(distance).value() * distance_centimeters_to_microsecond);
        return {pulse_length};
    }

    return std::nullopt;
}

}
