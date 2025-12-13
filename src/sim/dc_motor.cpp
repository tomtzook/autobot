
#include "sim/dc_motor.h"


namespace autobot::sim {

dc_motor::dc_motor(const units::volts_per_rad_per_sec kv, const units::volts_per_rad_per_sec_per_sec ka)
    : m_plant(math::dc_motor_system(kv, ka))
{}

dc_motor::dc_motor(const math::dc_motor& motor, const units::jkg_meters_squared moment_of_inertia, const math::floating_type gearing)
    : m_plant(math::dc_motor_system(motor, moment_of_inertia, gearing))
{}

void dc_motor::input_voltage(const units::volts volts) {
    m_plant.input(math::dc_motor_system::input_voltage) = volts.value();
}

units::radians dc_motor::output_position() const {
    return units::radians(m_plant.output(math::dc_motor_system::output_position));
}

units::radians_per_second dc_motor::output_velocity() const {
    return units::radians_per_second(m_plant.output(math::dc_motor_system::output_velocity));
}

void dc_motor::update(const units::seconds dt) {
    m_plant.update(dt.value());
}

}
