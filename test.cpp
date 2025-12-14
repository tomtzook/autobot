
#include <chrono>
#include <iostream>

#include "units.h"
#include "sim/dc_motor.h"
#include "math/control/trapezoid_profile.h"

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

    autobot::units::meters_per_second mv(1);
    autobot::units::meters_per_second_squared ma(1);
    autobot::math::trapezoid_profile<autobot::units::meters> profile(mv, ma);

    motor_sim.input_voltage(6.0_v);

    autobot::units::seconds time(0);
    while (true) {
        /*motor_sim.update(0.02_s);

        const auto pos = motor_sim.output_position();
        std::cout << autobot::units::convert<autobot::units::degrees>(pos).value() << std::endl;
        std::cout << autobot::units::convert<autobot::units::rotations>(pos).value() << std::endl;*/

        const auto state = profile.calculate(2.0_m, time);
        std::cout << "SP pos=" << state.position.value() << ", vel=" << state.velocity.value() << std::endl;

        time += 1.0_s;
        sleep(1);
    }

    return 0;
}