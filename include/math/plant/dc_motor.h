#pragma once

#include "math/common.h"
#include "math/plant/plant.h"

namespace autobot::math {

// 1 / kA = gearing * motor.KtNMPerAmp / (motor.rOhms * JKgMetersSquared)
// kA = (motor.rOhms * JKgMetersSquared) / (gearing * motor.KtNMPerAmp)
// -kV / kA = -gearing * gearing * motor.KtNMPerAmp / (motor.KvRadPerSecPerVolt * motor.rOhms * JKgMetersSquared)
// -kV / kA = -gearing / (kA * motor.KvRadPerSecPerVolt)
// kV = gearing / motor.KvRadPerSecPerVolt

struct dc_motor {
    floating_type nominal_voltage_volts;
    floating_type stall_torque_newton_meters;
    floating_type stall_current_amps;
    floating_type freeCurrentAmps;
    floating_type free_speed_rad_per_sec;
    floating_type kt_nm_per_amp;
    floating_type r_ohms;
    floating_type kv_rad_per_sec_per_volt;

    dc_motor(floating_type nominal_voltage_volts,
             floating_type stall_torque_newton_meters,
             floating_type stall_current_amps,
             floating_type free_current_amps,
             floating_type free_speed_rad_per_sec);

    [[nodiscard]] floating_type kv(floating_type gearing) const;
    [[nodiscard]] floating_type ka(floating_type gearing, floating_type moment_of_inertia) const;
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

    dc_motor_system(type kv, type ka);
    dc_motor_system(const dc_motor& motor, type moment_of_inertia, type gearing);
};

using dc_motor_plant = plant<floating_type, 0, 1, 2, 2, dc_motor_system>;

}
