
#include <chrono>
#include <iostream>

#include "units.h"
#include "sim/dc_motor.h"
#include "math/control/trapezoid_profile.h"
#include "math/ops.h"
#include "math/control/pid.h"

int main() {
    using namespace autobot::units::literals;

    static constexpr auto motor_moment_of_inertia = autobot::units::jkg_meters_squared(8.801E-4f);
    static constexpr auto motor_gearing = 1.f;
    static constexpr auto pid_kp = 1;
    static constexpr auto pid_ki = 0;
    static constexpr auto pid_kd = 0;
    static constexpr auto pid_min_out = 0;
    static constexpr auto pid_max_out = 0;
    static constexpr auto profile_mv = autobot::units::radians_per_second(1);
    static constexpr auto profile_ma = autobot::units::radians_per_second_squared(1);
    static constexpr auto sleep_time = autobot::units::convert<autobot::units::microseconds>(200.0_ms);

    const auto target_position = autobot::units::convert<autobot::units::radians>(2.0_rot);

    const autobot::math::dc_motor motor{
        12.0_v,
        autobot::units::newton_meter(0.43),
        53.0_amps, 1.8_amps, 1380.206372_rad_s};

    autobot::sim::dc_motor motor_sim(motor, motor_moment_of_inertia, motor_gearing);

    autobot::math::pid_controller<autobot::units::radians> pid(pid_kp, pid_ki, pid_kd);
    pid.min_out(pid_min_out);
    pid.max_out(pid_max_out);
    pid.reset();

    autobot::math::trapezoid_profile<autobot::units::radians> profile(profile_mv, profile_ma);

    autobot::units::seconds time(0);
    while (true) {
        const auto state = profile.calculate(target_position, time);
        const auto out_volts = pid.calculate(
            motor_sim.output_position(),
            state.position,
            autobot::units::convert<autobot::units::seconds>(sleep_time)
        );

        motor_sim.input_voltage(autobot::units::volts(out_volts));
        motor_sim.update(autobot::units::convert<autobot::units::seconds>(sleep_time));

        std::cout <<
            autobot::units::convert<autobot::units::rotations>(motor_sim.output_position()) << std::endl;

        time += sleep_time;
        usleep(sleep_time.value());
    }

    return 0;
}