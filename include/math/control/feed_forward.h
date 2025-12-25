#pragma once

#include "math/common.h"
#include "units.h"
#include "math/ops.h"

namespace autobot::math {

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
class static_feed_forward {
public:
    using unit = units_actual(unit_);
    using velocity_unit = units_per_second(unit);
    using raw_type = unit::type;
    using type = units::measure<unit>;
    using velocity_type = units::measure<velocity_unit>;

    using static_gain = units::volts;

    static_feed_forward();
    explicit static_feed_forward(const static_gain& ks);

    [[nodiscard]] static_gain ks() const;
    void ks(const static_gain& ks);

    units::volts calculate(const velocity_type& current);

private:
    static_gain m_ks;
};

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
class motor_feed_forward {
public:
    using unit = units_actual(unit_);
    using velocity_unit = units_per_second(unit);
    using acceleration_unit = units_per_second(velocity_unit);
    using raw_type = unit::type;
    using type = units::measure<unit>;
    using velocity_type = units::measure<velocity_unit>;
    using acceleration_type = units::measure<acceleration_unit>;

    using velocity_gain = units::measure<units_per(units::units::volts, velocity_unit)>;
    using acceleration_gain = units::measure<units_per(units::units::volts, acceleration_unit)>;

    static constexpr auto neg_min_time = units::measure<units::unit_inverse<units::units::seconds>>(-1e-9);
    static constexpr auto one = raw_type(1);

    motor_feed_forward();
    motor_feed_forward(const velocity_gain& kv, const acceleration_gain& ka);
    motor_feed_forward(const dc_motor& motor, units::jkg_meters_squared moment_of_inertia, raw_type gearing);

    [[nodiscard]] velocity_gain kv() const;
    void kv(const velocity_gain& kv);
    [[nodiscard]] acceleration_gain ka() const;
    void ka(const acceleration_gain& ka);

    units::volts calculate(const velocity_type& current, const velocity_type& wanted, units::seconds dt);

private:
    velocity_gain m_kv;
    acceleration_gain m_ka;
};

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
static_feed_forward<unit_>::static_feed_forward()
    : m_ks(0)
{}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
static_feed_forward<unit_>::static_feed_forward(const static_gain& ks)
    : m_ks(ks)
{}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
static_feed_forward<unit_>::static_gain static_feed_forward<unit_>::ks() const {
    return m_ks;
}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
void static_feed_forward<unit_>::ks(const static_gain& ks) {
    m_ks = ks;
}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
units::volts static_feed_forward<unit_>::calculate(const velocity_type& current) {
    return m_ks * signum(current);
}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
motor_feed_forward<unit_>::motor_feed_forward()
    : m_kv(0)
    , m_ka(0)
{}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
motor_feed_forward<unit_>::motor_feed_forward(const velocity_gain& kv, const acceleration_gain& ka)
    : m_kv(kv)
    , m_ka(ka)
{}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
motor_feed_forward<unit_>::motor_feed_forward(const dc_motor& motor, units::jkg_meters_squared moment_of_inertia, raw_type gearing)
    : m_kv(motor.kv(gearing))
    , m_ka(motor.ka(gearing, moment_of_inertia))
{}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
motor_feed_forward<unit_>::velocity_gain motor_feed_forward<unit_>::kv() const {
    return m_kv;
}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
void motor_feed_forward<unit_>::kv(const velocity_gain& kv) {
    m_kv = kv;
}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
motor_feed_forward<unit_>::acceleration_gain motor_feed_forward<unit_>::ka() const {
    return m_ka;
}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
void motor_feed_forward<unit_>::ka(const acceleration_gain& ka) {
    m_ka = ka;
}

template<units::unit_of_category_type<units::category::length, units::category::angle> unit_>
units::volts motor_feed_forward<unit_>::calculate(const velocity_type& current, const velocity_type& wanted, units::seconds dt) {
    const auto A = -m_kv / m_ka;
    const auto B = one / m_ka;
    const auto A_d = exp(A * dt);
    const auto B_d = A > neg_min_time ? B * dt : one / A * (A_d - one) * B;
    return one / B_d * (wanted - A_d * current);
}

}
