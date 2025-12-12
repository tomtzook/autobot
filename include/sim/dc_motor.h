#pragma once

#include "math/plant/dc_motor.h"

namespace autobot::sim {

class dc_motor {
public:
    dc_motor(math::floating_type kv, math::floating_type ka);
    dc_motor(const math::dc_motor& motor, math::floating_type moment_of_inertia, math::floating_type gearing);

    void input_voltage(math::floating_type volts);

    [[nodiscard]] math::floating_type output_position() const;
    [[nodiscard]] math::floating_type output_velocity() const;

    void update(math::floating_type dt);

private:
    math::dc_motor_plant m_plant;
};


}
