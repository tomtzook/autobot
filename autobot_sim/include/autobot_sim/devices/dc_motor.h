#pragma once

#include <autobot/math/plant/dc_motor.h>
#include "autobot_sim/dynamics/body.h"

namespace autobot::sim::devices {

class dc_motor {
public:
    using joint = dynamics::joint<dynamics::revolute_joint>;

    dc_motor(const math::dc_motor& motor, math::floating_type gear_ratio, const joint& joint);

    void set(units::volts voltage);
    void update();

private:
    math::dc_motor m_motor;
    math::floating_type m_gear_ratio;
    joint m_joint;

    units::volts m_last_input;
};

}
