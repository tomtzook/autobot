
#include "math/plant/dc_motor.h"

namespace autobot::math {

dc_motor::dc_motor(
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

units::volts_per_rad_per_sec dc_motor::kv(const floating_type gearing) const {
    return units::volts_per_rad_per_sec(gearing / motor_kv.value());
}

units::volts_per_rad_per_sec_per_sec dc_motor::ka(const floating_type gearing, const units::jkg_meters_squared moment_of_inertia) const {
    return units::volts_per_rad_per_sec_per_sec((resistence.value() * moment_of_inertia.value()) / (gearing * motor_kt.value()));
}

dc_motor_system::dc_motor_system(const units::volts_per_rad_per_sec kv, const units::volts_per_rad_per_sec_per_sec ka)
    : state_spaced_system(
        matrix2f{{0, 1}, {0, -kv.value() / ka.value()}},
        vector2f{0, 1 / ka.value()},
        matrix2f{{1, 0}, {0, 1}},
        vector2f{0, 0}
    )
{}

dc_motor_system::dc_motor_system(const dc_motor& motor, const units::jkg_meters_squared moment_of_inertia, const type gearing)
    : dc_motor_system(motor.kv(gearing), motor.ka(gearing, moment_of_inertia))
{}

}
