
#include "math/plant/dc_motor.h"

namespace autobot::math {

dc_motor::dc_motor(
        const floating_type nominal_voltage_volts,
        const floating_type stall_torque_newton_meters,
        const floating_type stall_current_amps,
        const floating_type free_current_amps,
        const floating_type free_speed_rad_per_sec)
     : nominal_voltage_volts(nominal_voltage_volts)
     , stall_torque_newton_meters(stall_torque_newton_meters)
     , stall_current_amps(stall_current_amps)
     , freeCurrentAmps(free_current_amps)
     , free_speed_rad_per_sec(free_speed_rad_per_sec)
     , kt_nm_per_amp(stall_torque_newton_meters / stall_current_amps)
     , r_ohms(nominal_voltage_volts / stall_current_amps)
     , kv_rad_per_sec_per_volt(free_speed_rad_per_sec / (nominal_voltage_volts - r_ohms * free_current_amps))
{}

floating_type dc_motor::kv(const floating_type gearing) const {
    return gearing / kv_rad_per_sec_per_volt;
}

floating_type dc_motor::ka(const floating_type gearing, const floating_type moment_of_inertia) const {
    return (r_ohms * moment_of_inertia) / (gearing * kt_nm_per_amp);
}

dc_motor_system::dc_motor_system(const type kv, const type ka)
    : state_spaced_system(
        matrix2f{{0, 1}, {0, -kv / ka}},
        vector2f{0, 1 / ka},
        matrix2f{{1, 0}, {0, 1}},
        vector2f{0, 0}
    )
{}

dc_motor_system::dc_motor_system(const dc_motor& motor, const type moment_of_inertia, const type gearing)
    : dc_motor_system(motor.kv(gearing), motor.ka(gearing, moment_of_inertia))
{}

}
