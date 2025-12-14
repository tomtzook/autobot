#pragma once

#include <ratio>

#include "math/common.h"

namespace autobot::units {
namespace detail {

struct unit_base_ {};
struct category_base_ {};
struct measure_base_ {};

template<typename t_>
concept unit_type = std::is_base_of_v<unit_base_, t_>;

template<typename t_>
struct is_category_base : std::is_base_of<category_base_, t_> {};
template<typename t_>
inline constexpr bool is_category_base_v = std::is_base_of_v<category_base_, t_>;
template<typename t_>
concept category_base_type = is_category_base_v<t_>;

template<typename t_>
concept measure_type = std::is_base_of_v<measure_base_, t_>;

template<typename t_>
concept ratio_type = std::__is_ratio_v<t_>;

template<ratio_type ratio_, ratio_type pi_exponent_ = std::ratio<0>>
struct converter {
    using ratio = ratio_;
    using pi_exponent = pi_exponent_;
};

template<unit_type t_>
struct unit_name {
    static constexpr auto name() noexcept {
        return "";
    }
};

template<
    ratio_type length_ = std::ratio<0>,
    ratio_type time_ = std::ratio<0>,
    ratio_type angle_ = std::ratio<0>,
    ratio_type voltage_ = std::ratio<0>,
    ratio_type current_ = std::ratio<0>,
    ratio_type mass_ = std::ratio<0>
    >
struct category_base : category_base_ {
    using length = length_;
    using time = time_;
    using angle = angle_;
    using voltage = voltage_;
    using current = current_;
    using mass = mass_;
};

template<class, class> struct category_base_multiply_impl;
template<class... exponents1_, class... exponents2_>
struct category_base_multiply_impl<category_base<exponents1_...>, category_base<exponents2_...>> {
    using type = category_base<std::ratio_add<exponents1_, exponents2_>...>;
};

template<class cat1_, class cat2_>
using category_base_multiply = category_base_multiply_impl<cat1_, cat2_>::type;

template<class, class> struct category_base_divide_impl;
template<class... exponents1_, class... exponents2_>
struct category_base_divide_impl<category_base<exponents1_...>, category_base<exponents2_...>> {
    using type = category_base<std::ratio_subtract<exponents1_, exponents2_>...>;
};

template<class cat1_, class cat2_>
using category_base_divide = category_base_divide_impl<cat1_, cat2_>::type;

template<class> struct inverse_category_base_impl;
template<class... exponents_>
struct inverse_category_base_impl<category_base<exponents_...>> {
    using type = category_base<std::ratio_multiply<exponents_, std::ratio<-1>>...>;
};

template<class cat_>
using inverse_category_base = inverse_category_base_impl<cat_>::type;

template<class> struct squared_category_base_impl;
template<class... exponents_>
struct squared_category_base_impl<category_base<exponents_...>> {
    using type = category_base<std::ratio_multiply<exponents_, std::ratio<2>>...>;
};

template<class cat_>
using squared_category_base = squared_category_base_impl<cat_>::type;

}

template<math::numeric type_, detail::category_base_type cat_, typename converter_>
struct unit : detail::unit_base_ {
    using type = type_;
    using cat = cat_;
    using converter = converter_;
};

template<detail::unit_type base_unit_, typename converter_>
struct derived_unit : detail::unit_base_ {
    using type = base_unit_::type;
    using cat = base_unit_::cat;
    using converter = detail::converter<
        std::ratio_multiply<typename base_unit_::converter::ratio, typename converter_::ratio>,
        std::ratio_add<typename base_unit_::converter::pi_exponent, typename converter_::pi_exponent>
        >;
};

template<typename t_>
concept unit_type = detail::unit_type<t_>;
template<typename t_>
concept measure_type = detail::measure_type<t_>;
template<typename t_>
concept unit_or_measure_type = detail::unit_type<t_> || detail::measure_type<t_>;

template<typename t_>
struct underlying_unit;
template<detail::unit_type t_>
struct underlying_unit<t_> { using type = t_; };
template<detail::measure_type t_>
struct underlying_unit<t_> { using type = t_::unit; };

template<detail::unit_type unit_>
struct measure;

namespace ops {

template<detail::unit_type unit1_, detail::unit_type unit2_>
struct unit_multiply_impl {
    static_assert(std::is_same_v<typename unit1_::type, typename unit2_::type>, "units must use the same underlying types");

    using type = unit<
            typename unit1_::type,
            detail::category_base_multiply<typename unit1_::cat, typename unit2_::cat>,
            detail::converter<
                std::ratio_multiply<typename unit1_::converter::ratio, typename unit2_::converter::ratio>,
                std::ratio_add<typename unit1_::converter::pi_exponent, typename unit2_::converter::pi_exponent>
            >
        >;
};

template<detail::unit_type unit1_, detail::unit_type unit2_>
struct unit_divide_impl {
    static_assert(std::is_same_v<typename unit1_::type, typename unit2_::type>, "units must use the same underlying types");

    using type = unit<
            typename unit1_::type,
            detail::category_base_divide<typename unit1_::cat, typename unit2_::cat>,
            detail::converter<
                std::ratio_divide<typename unit1_::converter::ratio, typename unit2_::converter::ratio>,
                std::ratio_subtract<typename unit1_::converter::pi_exponent, typename unit2_::converter::pi_exponent>
            >
        >;
};

template<detail::unit_type unit_>
struct unit_inverse_impl {
    using type = unit<
            typename unit_::type,
            detail::inverse_category_base<typename unit_::cat>,
            detail::converter<
                std::ratio<unit_::converter::ratio::den, unit_::converter::ratio::num>,
                std::ratio_multiply<typename unit_::converter::pi_exponent, std::ratio<-1>>
            >
        >;
};

template<detail::unit_type unit_>
struct unit_squared_impl {
    using type = unit<
            typename unit_::type,
            detail::squared_category_base<typename unit_::cat>,
            detail::converter<
                std::ratio_multiply<typename unit_::converter::ratio, typename unit_::converter::ratio>,
                std::ratio_multiply<typename unit_::converter::pi_exponent, std::ratio<2>>
            >
        >;
};

template<detail::unit_type unit1_, detail::unit_type unit2_>
inline constexpr bool is_convertible_v = std::is_same_v<typename unit1_::cat, typename unit2_::cat>;

template<detail::unit_type dst_unit_, detail::unit_type src_unit_>
constexpr measure<dst_unit_> convert(const measure<src_unit_>& src) noexcept requires(ops::is_convertible_v<src_unit_, dst_unit_>) {
    if constexpr (std::is_same_v<dst_unit_, src_unit_>) {
        return src;
    } else {
        using ratio = std::ratio_divide<typename src_unit_::converter::ratio, typename dst_unit_::converter::ratio>;
        using pi_ratio = std::ratio_subtract<typename src_unit_::converter::pi_exponent, typename dst_unit_::converter::pi_exponent>;
        using type = dst_unit_::type;

        auto converted = static_cast<type>(src.value()) * static_cast<type>(ratio::num) / static_cast<type>(ratio::den);
        if constexpr (pi_ratio::num != 0) {
            const auto pi = std::pow(math::pi, static_cast<type>(pi_ratio::num) / static_cast<type>(pi_ratio::den));
            converted *= pi;
        }

        return measure<dst_unit_>(converted);
    }
}

}

template<detail::unit_type unit1_, detail::unit_type unit2_>
using unit_multiply = ops::unit_multiply_impl<unit1_, unit2_>::type;

template<detail::unit_type unit1_, detail::unit_type unit2_>
using unit_divide = ops::unit_divide_impl<unit1_, unit2_>::type;

template<detail::unit_type unit_>
using unit_inverse = ops::unit_inverse_impl<unit_>::type;

template<detail::unit_type unit_>
using unit_squared = ops::unit_squared_impl<unit_>::type;

namespace detail {

template<unit_type unit_, unit_type... other_units_>
struct compound_unit_impl;
template<unit_type unit_>
struct compound_unit_impl<unit_> : unit_base_ { using type = unit_; };
template<unit_type unit1_, unit_type unit2_, unit_type... other_units_>
struct compound_unit_impl<unit1_, unit2_, other_units_...>
    : compound_unit_impl<unit_multiply<unit1_, unit2_>, other_units_...> {};

}

template<detail::unit_type unit1_, detail::unit_type unit2_, detail::unit_type... other_units_>
using compound_unit = detail::compound_unit_impl<unit1_, unit2_, other_units_...>::type;

template<typename t_, typename... cats_>
concept unit_of_category_type =
    (detail::unit_type<t_> || detail::measure_type<t_>) &&
    std::conjunction_v<detail::is_category_base<cats_>...> &&
    std::disjunction_v<std::is_same<typename underlying_unit<t_>::type::cat, cats_>...>;

namespace category {

using length = detail::category_base<std::ratio<1>>;
using time = detail::category_base<std::ratio<0>, std::ratio<1>>;
using angle = detail::category_base<std::ratio<0>, std::ratio<0>, std::ratio<1>>;
using voltage = detail::category_base<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;
using current = detail::category_base<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;
using mass = detail::category_base<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;

using force = detail::category_base<std::ratio<1>, std::ratio<-2>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;
using energy = detail::category_base<std::ratio<2>, std::ratio<-2>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>>;

using linear_velocity = detail::category_base<std::ratio<1>, std::ratio<-1>>;
using angular_velocity = detail::category_base<std::ratio<0>, std::ratio<-1>, std::ratio<1>>;

using linear_acceleration = detail::category_base<std::ratio<1>, std::ratio<-2>>;
using angular_acceleration = detail::category_base<std::ratio<0>, std::ratio<-2>, std::ratio<1>>;

}

template<typename dst_unit_, detail::unit_type src_unit_>
constexpr auto convert(const measure<src_unit_>& src) noexcept {
    return ops::convert<typename underlying_unit<dst_unit_>::type, src_unit_>(src);
}

template<detail::unit_type unit_>
struct measure : detail::measure_base_ {
    using unit = unit_;
    using type = unit::type;

    measure() = default;
    measure(const measure&) = default;
    measure(measure&&) = default;

    explicit constexpr measure(const type value) : m_value(value) {}

    measure& operator=(const measure&) = default;
    measure& operator=(measure&&) = default;

    explicit constexpr operator type() const noexcept { return m_value; }
    [[nodiscard]] constexpr type value() const noexcept { return m_value; }

    constexpr measure& operator=(const type value) noexcept { m_value = value; return *this; }
    constexpr void value(const type value) noexcept { m_value = value; }

    template<detail::unit_type other_unit_>
    constexpr measure& operator=(const measure<other_unit_>& rhs) noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { m_value = convert<unit>(rhs).m_value; return *this; };

    template<detail::unit_type other_unit_>
    constexpr bool operator==(const measure<other_unit_>& rhs) const noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { return m_value == convert<unit>(rhs).m_value; };
    template<detail::unit_type other_unit_>
    constexpr bool operator!=(const measure<other_unit_>& rhs) const noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { return m_value != convert<unit>(rhs).m_value; };
    template<detail::unit_type other_unit_>
    constexpr bool operator>(const measure<other_unit_>& rhs) const noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { return m_value > convert<unit>(rhs).m_value; };
    template<detail::unit_type other_unit_>
    constexpr bool operator<(const measure<other_unit_>& rhs) const noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { return m_value < convert<unit>(rhs).m_value; };
    template<detail::unit_type other_unit_>
    constexpr bool operator>=(const measure<other_unit_>& rhs) const noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { return m_value >= convert<unit>(rhs).m_value; };
    template<detail::unit_type other_unit_>
    constexpr bool operator<=(const measure<other_unit_>& rhs) const noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { return m_value <= convert<unit>(rhs).m_value; };

    template<detail::unit_type other_unit_>
    constexpr measure operator+(const measure<other_unit_>& rhs) const noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { return measure{m_value + convert<unit>(rhs).m_value}; }
    template<detail::unit_type other_unit_>
    constexpr measure operator-(const measure<other_unit_>& rhs) const noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { return measure{m_value - convert<unit>(rhs).m_value}; }
    template<detail::unit_type other_unit_>
    constexpr measure<compound_unit<unit, other_unit_>> operator*(const measure<other_unit_>& rhs) const noexcept requires(!ops::is_convertible_v<unit, other_unit_>)
    { return measure<compound_unit<unit, other_unit_>>{m_value * rhs.value()}; }
    template<detail::unit_type other_unit_>
    constexpr measure<compound_unit<unit, unit_inverse<other_unit_>>> operator/(const measure<other_unit_>& rhs) const noexcept requires(!ops::is_convertible_v<unit, other_unit_>)
    { return measure<compound_unit<unit, unit_inverse<other_unit_>>>{m_value / rhs.value()}; }

    constexpr measure operator*(const type rhs) const noexcept { return measure{m_value * rhs}; }
    constexpr measure operator/(const type& rhs) const noexcept { return measure{m_value / rhs}; }

    template<detail::unit_type other_unit_>
    constexpr measure& operator+=(const measure<other_unit_>& rhs) noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { m_value += convert<unit>(rhs).m_value; return *this; }
    template<detail::unit_type other_unit_>
    constexpr measure& operator-=(const measure<other_unit_>& rhs) noexcept requires(ops::is_convertible_v<unit, other_unit_>)
    { m_value -= convert<unit>(rhs).m_value; return *this; }
    constexpr measure& operator*=(const type rhs) const noexcept { m_value *= rhs; return *this; }
    constexpr measure& operator/=(const type rhs) const noexcept { m_value /= rhs; return *this; }

private:
    type m_value;
};

template<detail::unit_type unit_>
constexpr measure<unit_> operator*(const typename unit_::type lhs, const measure<unit_>& rhs) noexcept { return measure<unit_>{lhs * rhs.value()}; }
template<detail::unit_type unit_>
constexpr measure<unit_> operator/(const typename unit_::type lhs, const measure<unit_>& rhs) noexcept { return measure<unit_>{lhs / rhs.value()}; }

#define create_unit(_name, ...) \
    namespace units { \
        using _name = __VA_ARGS__ ; \
    }\
    template<> struct ::autobot::units::detail::unit_name< units::_name > { \
        static constexpr auto name() noexcept { \
            return #_name; \
        }\
    }; \
    using _name = ::autobot::units::measure<units:: _name >;

// base units
create_unit(meters, unit<math::floating_type, category::length, detail::converter<std::ratio<1>>>)
create_unit(seconds, unit<math::floating_type, category::time, detail::converter<std::ratio<1>>>)
create_unit(radians, unit<math::floating_type, category::angle, detail::converter<std::ratio<1>>>)
create_unit(volts, unit<math::floating_type, category::voltage, detail::converter<std::ratio<1>>>)
create_unit(ampere, unit<math::floating_type, category::current, detail::converter<std::ratio<1>>>)
create_unit(kilogram, unit<math::floating_type, category::mass, detail::converter<std::ratio<1>>>)
create_unit(newton, unit<math::floating_type, category::force, detail::converter<std::ratio<1>>>)
create_unit(joule, unit<math::floating_type, category::energy, detail::converter<std::ratio<1>>>)

// length
create_unit(nanometers, derived_unit<meters, detail::converter<std::nano>>)
create_unit(micrometers, derived_unit<meters, detail::converter<std::micro>>)
create_unit(millimeters, derived_unit<meters, detail::converter<std::milli>>)
create_unit(centimeters, derived_unit<meters, detail::converter<std::centi>>)
create_unit(kilometers, derived_unit<meters, detail::converter<std::kilo>>)

// time
create_unit(nanoseconds, derived_unit<seconds, detail::converter<std::nano>>)
create_unit(microseconds, derived_unit<seconds, detail::converter<std::micro>>)
create_unit(milliseconds, derived_unit<seconds, detail::converter<std::milli>>)
create_unit(minutes, derived_unit<seconds, detail::converter<std::ratio<60>>>)
create_unit(hours, derived_unit<seconds, detail::converter<std::ratio<3600>>>)

// angle
create_unit(degrees, derived_unit<radians, detail::converter<std::ratio<1, 180>, std::ratio<1>>>)
create_unit(rotations, derived_unit<radians, detail::converter<std::ratio<2>, std::ratio<1>>>)

// volts


// current
using amps = ampere;

// mass


// force


// energy


// linear velocity
create_unit(meters_per_second, compound_unit<meters, unit_inverse<seconds>>)

// angular velocity
create_unit(radians_per_second, compound_unit<radians, unit_inverse<seconds>>);
create_unit(degrees_per_second, compound_unit<degrees, unit_inverse<seconds>>);
create_unit(rotations_per_second, compound_unit<rotations, unit_inverse<seconds>>);
create_unit(rotations_per_minute, compound_unit<rotations, unit_inverse<minutes>>);

using rps = rotations_per_second;
using rpm = rotations_per_minute;

// linear acceleration
create_unit(meters_per_second_squared, compound_unit<meters_per_second, unit_inverse<seconds>>)

// angular acceleration
create_unit(radians_per_second_squared, compound_unit<radians_per_second, unit_inverse<seconds>>);
create_unit(degrees_per_second_squared, compound_unit<degrees_per_second, unit_inverse<seconds>>);
create_unit(rotations_per_second_squared, compound_unit<rotations_per_second, unit_inverse<seconds>>);
create_unit(rotations_per_minute_per_second, compound_unit<rotations_per_minute, unit_inverse<seconds>>)

// resistence
create_unit(ohm, compound_unit<volts, unit_inverse<ampere>>);

// torque
// todo: fix
// create_unit(newton_meter, compound_unit<newton, meters>);
namespace units { using newton_meter = joule; }
using newton_meter = joule;

// moment of inertia
create_unit(jkg_meters_squared, compound_unit<joule, kilogram, unit_squared<meters>>);

// other
create_unit(rad_per_sec_per_volt, compound_unit<radians_per_second, unit_inverse<volts>>);
create_unit(newton_meter_per_amp, compound_unit<newton_meter, unit_inverse<ampere>>);
create_unit(volts_per_rad_per_sec, compound_unit<volts, unit_inverse<radians_per_second>>)
create_unit(volts_per_rad_per_second_squared, compound_unit<volts, unit_inverse<radians_per_second_squared>>)

namespace literals {

constexpr nanometers operator""_nm(const long double value) noexcept { return nanometers{static_cast<math::floating_type>(value)}; }
constexpr micrometers operator""_um(const long double value) noexcept { return micrometers{static_cast<math::floating_type>(value)}; }
constexpr millimeters operator""_mm(const long double value) noexcept { return millimeters{static_cast<math::floating_type>(value)}; }
constexpr centimeters operator""_cm(const long double value) noexcept { return centimeters{static_cast<math::floating_type>(value)}; }
constexpr meters operator""_m(const long double value) noexcept { return meters{static_cast<math::floating_type>(value)}; }
constexpr kilometers operator""_km(const long double value) noexcept { return kilometers{static_cast<math::floating_type>(value)}; }

constexpr nanoseconds operator""_ns(const long double value) noexcept { return nanoseconds{static_cast<math::floating_type>(value)}; }
constexpr microseconds operator""_us(const long double value) noexcept { return microseconds{static_cast<math::floating_type>(value)}; }
constexpr milliseconds operator""_ms(const long double value) noexcept { return milliseconds{static_cast<math::floating_type>(value)}; }
constexpr seconds operator""_s(const long double value) noexcept { return seconds{static_cast<math::floating_type>(value)}; }
constexpr minutes operator""_min(const long double value) noexcept { return minutes{static_cast<math::floating_type>(value)}; }
constexpr hours operator""_h(const long double value) noexcept { return hours{static_cast<math::floating_type>(value)}; }

constexpr radians operator""_rad(const long double value) noexcept { return radians{static_cast<math::floating_type>(value)}; }
constexpr degrees operator""_deg(const long double value) noexcept { return degrees{static_cast<math::floating_type>(value)}; }
constexpr rotations operator""_rot(const long double value) noexcept { return rotations{static_cast<math::floating_type>(value)}; }

constexpr volts operator""_v(const long double value) noexcept { return volts{static_cast<math::floating_type>(value)}; }

constexpr ampere operator""_amps(const long double value) noexcept { return ampere{static_cast<math::floating_type>(value)}; }

constexpr meters_per_second operator""_m_s(const long double value) noexcept { return meters_per_second{static_cast<math::floating_type>(value)}; }
constexpr meters_per_second operator""_mps(const long double value) noexcept { return meters_per_second{static_cast<math::floating_type>(value)}; }

constexpr radians_per_second operator""_rad_s(const long double value) noexcept { return radians_per_second{static_cast<math::floating_type>(value)}; }
constexpr degrees_per_second operator""_deg_s(const long double value) noexcept { return degrees_per_second{static_cast<math::floating_type>(value)}; }
constexpr rotations_per_second operator""_rot_s(const long double value) noexcept { return rotations_per_second{static_cast<math::floating_type>(value)}; }
constexpr rotations_per_second operator""_rps(const long double value) noexcept { return rotations_per_second{static_cast<math::floating_type>(value)}; }
constexpr rotations_per_minute operator""_rpm(const long double value) noexcept { return rotations_per_minute{static_cast<math::floating_type>(value)}; }

}

}

template<autobot::units::detail::unit_type unit_>
std::ostream& operator<<(std::ostream& os, const autobot::units::measure<unit_>& value) {
    os << value.value() << " " << autobot::units::detail::unit_name<unit_>::name();
    return os;
}
