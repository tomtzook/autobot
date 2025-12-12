
#include "sim/dc_motor.h"


namespace autobot::sim {

dc_motor::dc_motor(const math::floating_type kv, const math::floating_type ka)
    : m_plant(math::dc_motor_system(kv, ka))
{}

dc_motor::dc_motor(const math::dc_motor& motor, const math::floating_type moment_of_inertia, const math::floating_type gearing)
    : m_plant(math::dc_motor_system(motor, moment_of_inertia, gearing))
{}

void dc_motor::input_voltage(const math::floating_type volts) {
    m_plant.input(math::dc_motor_system::input_voltage) = volts;
}

math::floating_type dc_motor::output_position() const {
    return m_plant.output(math::dc_motor_system::output_position);
}

math::floating_type dc_motor::output_velocity() const {
    return m_plant.output(math::dc_motor_system::output_velocity);
}

void dc_motor::update(const math::floating_type dt) {
    m_plant.update(dt);
}

}
