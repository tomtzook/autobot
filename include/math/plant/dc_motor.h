#pragma once

#include "math/common.h"
#include "math/plant/plant.h"
#include "units.h"

namespace autobot::math {

struct dc_motor {
    units::volts nominal_voltage;
    units::newton_meter stall_torque;
    units::amps stall_current;
    units::amps free_current;
    units::radians_per_second free_speed;
    units::newton_meter_per_amp motor_kt;
    units::ohm resistence;
    units::rad_per_sec_per_volt motor_kv;

    constexpr dc_motor(
            units::volts nominal_voltage,
            units::newton_meter stall_torque,
            units::amps stall_current,
            units::amps free_current,
            units::radians_per_second free_speed);

    [[nodiscard]] constexpr dc_motor times(size_t count) const;
    [[nodiscard]] constexpr units::volts_per_rad_per_sec kv(floating_type gearing) const;
    [[nodiscard]] constexpr units::volts_per_rad_per_second_squared ka(floating_type gearing, units::jkg_meters_squared moment_of_inertia) const;
};

class dc_motor_system : public state_spaced_system<floating_type, 0, 1, 2, 2> {
public:
    enum inputs {
        input_voltage = 0 // volts
    };

    enum states {
        state_position = 0, // rads
        state_velocity = 1 // rads/s
    };

    enum outputs {
        output_position = 0, // rads
        output_velocity = 1 // rads/s
    };

    dc_motor_system(units::volts_per_rad_per_sec kv, units::volts_per_rad_per_second_squared ka);
    dc_motor_system(const dc_motor& motor, units::jkg_meters_squared moment_of_inertia, type gearing);
};

using dc_motor_plant = plant<floating_type, 0, 1, 2, 2, dc_motor_system>;

constexpr dc_motor::dc_motor(
    const units::volts nominal_voltage,
    const units::newton_meter stall_torque,
    const units::amps stall_current,
    const units::amps free_current,
    const units::radians_per_second free_speed)
     : nominal_voltage(nominal_voltage)
     , stall_torque(stall_torque)
     , stall_current(stall_current)
     , free_current(free_current)
     , free_speed(free_speed)
     , motor_kt(stall_torque / stall_current)
     , resistence(nominal_voltage / stall_current)
     , motor_kv(free_speed / (nominal_voltage - resistence * free_current))
{}

constexpr dc_motor dc_motor::times(const size_t count) const {
    return dc_motor{nominal_voltage, stall_torque * count, stall_current * count, free_current * count, free_speed};
}

constexpr units::volts_per_rad_per_sec dc_motor::kv(const floating_type gearing) const {
    return units::volts_per_rad_per_sec(gearing / motor_kv.value());
}

constexpr units::volts_per_rad_per_second_squared dc_motor::ka(const floating_type gearing, const units::jkg_meters_squared moment_of_inertia) const {
    return units::volts_per_rad_per_second_squared((resistence.value() * moment_of_inertia.value()) / (gearing * motor_kt.value()));
}

}
