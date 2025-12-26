
#include "sim/dc_motor.h"


namespace autobot::sim {

dc_motor::dc_motor(const units::volts_per_rad_per_sec kv, const units::volts_per_rad_per_second_squared ka)
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

void dc_motor::bind_dashboard(dashboard::bind&& bind) {
    m_dashboard_bind = std::move(bind);

    m_dashboard_bind.set_type("sim::dc_motor");
    m_dashboard_bind.set_units("mixed");

    m_dashboard_bind.add_func<float>("input",
        [this]()->float { return m_plant.input(math::dc_motor_system::input_voltage); }, {});
    m_dashboard_bind.add_func<float>("output_position",
        [this]()->float { return m_plant.output(math::dc_motor_system::output_position); }, {});
    m_dashboard_bind.add_func<float>("output_velocity",
        [this]()->float { return m_plant.output(math::dc_motor_system::output_velocity); }, {});
}

}
