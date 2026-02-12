
#include <chrono>

#include "units.h"
#include "sim/dc_motor.h"
#include "math/control/trapezoid_profile.h"
#include "math/ops.h"
#include "math/control/pid.h"
#include "math/control/feed_forward.h"
#include "dashboard/object.h"
#include "dashboard/toggle.h"
#include "dashboard/canvas.h"

#include "hal.h"
#include "hal_sim.h"

using namespace autobot::units::literals;

namespace robot {

constexpr autobot::math::dc_motor motor{
    12.0_volt,
    2.6_ntm,
    105.0_amp,
    1.8_amp,
    594.38933_rad_per_s};

static constexpr auto motor_moment_of_inertia = 8.801E-4_jkg_msq;
static constexpr auto motor_gearing = 1.f;
static constexpr auto pid_kp = 0.01_volt_per_rad;
static constexpr auto pid_ki = 0.0_volt_per_rad;
static constexpr auto pid_kd = 0.0_volt_per_rad_per_s;
static constexpr auto pid_min_out = -12.0_volt;
static constexpr auto pid_max_out = 12.0_volt;
static constexpr auto profile_mv = 1.0_rad_per_s;
static constexpr auto profile_ma = 0.1_rad_per_sq;
static constexpr auto sleep_time = 20.0_ms;
static constexpr auto target_position = autobot::units::convert<autobot::units::radians>(2.0_rot);

autobot::sim::dc_motor motor_sim(motor, motor_moment_of_inertia, motor_gearing);

autobot::math::pid_controller<autobot::units::radians, autobot::units::volts> pid(pid_kp, pid_ki, pid_kd);
autobot::math::motor_feed_forward<autobot::units::radians> ff(motor, motor_moment_of_inertia, motor_gearing);
autobot::math::trapezoid_profile<autobot::units::radians> profile(profile_mv, profile_ma);

autobot::units::seconds g_time(0);
autobot::math::trapezoid_profile<autobot::units::radians>::state g_last_profile_sp;

autobot::dashboard::toggle run_toggle;
autobot::dashboard::canvas_group group1;
autobot::dashboard::canvas_group group2;

void init() {
    pid.min_out(pid_min_out);
    pid.max_out(pid_max_out);
    pid.reset();

    g_time = 0;

    group2.x(5.0_m);
    group2.y(5.0_m);

    auto& registry = autobot::dashboard::get_registry();
    //registry.add("pid", pid);
    //registry.add("ff", ff);
    //registry.add("profile", profile);
    //registry.add("sim", motor_sim);
    registry.add("run", run_toggle);
    registry.add("group1", group1);
    registry.add("group2", group2);

    {
        auto& line1 = group1.create_line("line1");
        line1.x(2.0_m);
        line1.y(1.0_m);
        line1.length(3.0_m);
        line1.angle(autobot::units::convert<autobot::units::radians>(90.0_deg));
        line1.background(autobot::dashboard::rgba(128, 255, 255, 255));

        auto& line2 = group1.create_line("line2");
        line2.x(2.0_m);
        line2.y(1.0_m);
        line2.length(2.0_m);
        line2.angle(autobot::units::convert<autobot::units::radians>(0.0_deg));
        line2.background(autobot::dashboard::rgba(255, 255, 255, 255));

        auto& rect1 = group1.create_rect("rect1");
        rect1.x(2.0_m);
        rect1.y(3.0_m);
        rect1.length(2.0_m);
        rect1.width(2.0_m);
        rect1.background(autobot::dashboard::rgba(255, 255, 255, 255));

        auto& circle1 = group2.create_circle("circle1");
        circle1.x(2.0_m);
        circle1.y(3.0_m);
        circle1.radius(2.0_m);
        circle1.background(autobot::dashboard::rgba(255, 255, 255, 255));
    }

    registry.update();

    sleep(5);
}

void update() {
    if (!run_toggle.get()) {
        auto& registry = autobot::dashboard::get_registry();
        registry.update();
        return;
    }

    group1.x(2.0_m);

    const auto state = profile.calculate(target_position, g_time);
    const auto pid_out = pid.calculate(
        motor_sim.output_position(),
        state.position,
        autobot::units::convert<autobot::units::seconds>(sleep_time)
    );
    const auto ff_out = ff.calculate(motor_sim.output_velocity(), state.velocity, autobot::units::convert<autobot::units::seconds>(sleep_time));

    motor_sim.input_voltage(pid_out + ff_out);
    motor_sim.update(autobot::units::convert<autobot::units::seconds>(sleep_time));

    g_time += sleep_time;

    auto& registry = autobot::dashboard::get_registry();
    registry.update();
}

}

namespace sim {

void init() {
    autobot::hal::initialize(autobot::hal::sim::initialize);

    autobot::hal::sim::define_port(1, "DIO1", autobot::hal::port_type_digital_output);
}

}

int main() {
    obsr::start_server(50000);
    robot::init();

    auto last_update_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    while (true) {
        if (auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            (now - last_update_time) >= std::chrono::milliseconds(20)) {
            last_update_time = now;

            robot::update();
        }

        usleep(20000);
    }

    return 0;
}
