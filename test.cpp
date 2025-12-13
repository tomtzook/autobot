
#include <chrono>
#include <iostream>

#include "units.h"
#include "sim/dc_motor.h"

#include "units.h"

int main() {
    using namespace autobot::units::literals;

    static constexpr auto motor_moment_of_inertia = autobot::units::jkg_meters_squared(8.801E-4f);
    static constexpr auto motor_gearing = 1.f;
    const autobot::math::dc_motor motor{
        12.0_v,
        autobot::units::newton_meter(0.43),
        53.0_amps, 1.8_amps, 1380.206372_rad_s};

    autobot::sim::dc_motor motor_sim(motor, motor_moment_of_inertia, motor_gearing);

    motor_sim.input_voltage(6.0_v);
    while (true) {
        motor_sim.update(0.02_s);

        const auto pos = motor_sim.output_position();
        std::cout << autobot::units::convert<autobot::units::degrees>(pos).value() << std::endl;
        std::cout << autobot::units::convert<autobot::units::rotations>(pos).value() << std::endl;

        sleep(1);
    }

    return 0;
}