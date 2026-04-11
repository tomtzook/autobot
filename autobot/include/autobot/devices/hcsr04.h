#pragma once

#include <hal.h>

#include "autobot/units.h"

namespace autobot::devices {

template<units::unit_of_category_type<units::category::length> unit_>
class hcsr04 {
public:
    using unit = units_actual(unit_);
    using type = units::measure<unit>;

    static constexpr auto trigger_pulse_duration_us = 6;
    static constexpr auto min_range = units::convert<units::meters>(units::centimeters(2));
    static constexpr auto max_range = units::meters(4);
    static constexpr auto distance_centimeters_to_microsecond = 58;

    hcsr04(hal::device_id trigger_id, hal::device_id echo_id);

    void trigger();
    std::optional<type> read_distance() const;

private:
    std::optional<units::centimeters> read_last_distance() const;

    hal::digital_output m_trigger;
    hal::pulse_width_reader m_echo;
};

template<units::unit_of_category_type<units::category::length> unit_>
hcsr04<unit_>::hcsr04(const hal::device_id trigger_id, const hal::device_id echo_id)
    : m_trigger(hal::open_device(trigger_id, hal::type_port_digital_output))
    , m_echo(hal::open_device(echo_id, hal::type_pulsewidth_reader))
{}

template<units::unit_of_category_type<units::category::length> unit_>
void hcsr04<unit_>::trigger() {
    m_trigger.pulse(trigger_pulse_duration_us);
}

template<units::unit_of_category_type<units::category::length> unit_>
std::optional<hcsr04<unit_>::type> hcsr04<unit_>::read_distance() const {
    const auto opt = read_last_distance();
    if (!opt) {
        return std::nullopt;
    }

    auto distance = units::convert<unit>(opt.value());
    if (distance > max_range || distance < min_range) {
        return std::nullopt;
    }

    return distance;
}

template<units::unit_of_category_type<units::category::length> unit_>
std::optional<units::centimeters> hcsr04<unit_>::read_last_distance() const {
    const auto pulse_duration = m_echo.read();
    if (pulse_duration < 1) {
        return std::nullopt;
    }

    return units::centimeters(pulse_duration / distance_centimeters_to_microsecond);
}

}
