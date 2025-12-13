#pragma once

#include "math/common.h"
#include "math/plant/plant.h"
#include "units.h"

namespace autobot::math {

// 1 / kA = gearing * motor.KtNMPerAmp / (motor.rOhms * JKgMetersSquared)
// kA = (motor.rOhms * JKgMetersSquared) / (gearing * motor.KtNMPerAmp)
// -kV / kA = -gearing * gearing * motor.KtNMPerAmp / (motor.KvRadPerSecPerVolt * motor.rOhms * JKgMetersSquared)
// -kV / kA = -gearing / (kA * motor.KvRadPerSecPerVolt)
// kV = gearing / motor.KvRadPerSecPerVolt

struct dc_motor {
    units::volts nominal_voltage;
    units::newton_meter stall_torque;
    units::amps stall_current;
    units::amps free_current;
    units::radians_per_second free_speed;
    units::newton_meter_per_amp motor_kt;
    units::ohm resistence;
    units::rad_per_sec_per_volt motor_kv;

    dc_motor(units::volts nominal_voltage,
             units::newton_meter stall_torque,
             units::amps stall_current,
             units::amps free_current,
             units::radians_per_second free_speed);

    [[nodiscard]] units::volts_per_rad_per_sec kv(floating_type gearing) const;
    [[nodiscard]] units::volts_per_rad_per_sec_per_sec ka(floating_type gearing, units::jkg_meters_squared moment_of_inertia) const;
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

    dc_motor_system(units::volts_per_rad_per_sec kv, units::volts_per_rad_per_sec_per_sec ka);
    dc_motor_system(const dc_motor& motor, units::jkg_meters_squared moment_of_inertia, type gearing);
};

using dc_motor_plant = plant<floating_type, 0, 1, 2, 2, dc_motor_system>;

}
