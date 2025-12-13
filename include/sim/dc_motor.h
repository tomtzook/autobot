#pragma once

#include "math/plant/dc_motor.h"
#include "units.h"

namespace autobot::sim {

class dc_motor {
public:
    dc_motor(units::volts_per_rad_per_sec kv, units::volts_per_rad_per_sec_per_sec ka);
    dc_motor(const math::dc_motor& motor, units::jkg_meters_squared moment_of_inertia, math::floating_type gearing);

    void input_voltage(units::volts volts);

    [[nodiscard]] units::radians output_position() const;
    [[nodiscard]] units::radians_per_second output_velocity() const;

    void update(units::seconds dt);

private:
    math::dc_motor_plant m_plant;
};


}
