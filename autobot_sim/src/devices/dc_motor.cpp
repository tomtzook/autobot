
#include "autobot_sim/devices/dc_motor.h"

namespace autobot::sim::devices {

dc_motor::dc_motor(const math::dc_motor& motor, const math::floating_type gear_ratio, const joint& joint)
    : m_motor(motor)
    , m_gear_ratio(gear_ratio)
    , m_joint(joint)
    , m_last_input(0)
{}

void dc_motor::set(const units::volts voltage) {
    m_last_input = voltage;
}

void dc_motor::update() {
    const auto velocity = m_joint.get_velocity();
    const auto torque = m_motor.torque(m_gear_ratio, m_last_input, velocity);
    m_joint.set_command(torque);
}


}
