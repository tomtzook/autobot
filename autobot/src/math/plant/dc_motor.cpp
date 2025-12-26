
#include "math/plant/dc_motor.h"

namespace autobot::math {

dc_motor_system::dc_motor_system(const units::volts_per_rad_per_sec kv, const units::volts_per_rad_per_second_squared ka)
    : state_spaced_system(
        matrix2f{{0, 1}, {0, -kv.value() / ka.value()}},
        vector2f{0, 1 / ka.value()},
        matrix2f{{1, 0}, {0, 1}},
        vector2f{0, 0}
    )
{}

dc_motor_system::dc_motor_system(const dc_motor& motor, const units::jkg_meters_squared moment_of_inertia, const type gearing)
    : dc_motor_system(motor.kv(gearing), motor.ka(gearing, moment_of_inertia))
{}

}
