#pragma once

#include <cmath>

#include "units.h"

namespace autobot::math {

template<units::unit_type unit_>
constexpr units::measure<unit_> constrain(const units::measure<unit_>& value, const units::measure<unit_>& min, const units::measure<unit_>& max) {
    if (value > max) {
        return max;
    }
    if (value < min) {
        return min;
    }

    return value;
}

template<units::unit_type unit_>
constexpr units::measure<unit_> abs(const units::measure<unit_>& value) {
    return units::measure<unit_>(std::abs(value.value()));
}

template<units::unit_type unit_>
constexpr units::measure<unit_> floor(const units::measure<unit_>& value) {
    return units::measure<unit_>(std::floor(value.value()));
}

template<units::unit_type unit_>
constexpr units::measure<unit_> ceil(const units::measure<unit_>& value) {
    return units::measure<unit_>(std::ceil(value.value()));
}

template<units::unit_of_category_type<units::category::angle> unit_>
constexpr floating_type sin(const units::measure<unit_>& value) {
    return static_cast<floating_type>(std::sin(units::convert<units::radians>(value).value()));
}

template<units::unit_of_category_type<units::category::angle> unit_>
constexpr floating_type cos(const units::measure<unit_>& value) {
    return static_cast<floating_type>(std::cos(units::convert<units::radians>(value).value()));
}

template<units::unit_of_category_type<units::category::angle> unit_>
constexpr floating_type tan(const units::measure<unit_>& value) {
    return static_cast<floating_type>(std::tan(units::convert<units::radians>(value).value()));
}

template<units::unit_of_category_type<units::category::angle> unit_ = units::radians>
constexpr auto asin(const floating_type value) {
    return units::convert<unit_>(units::radians(std::asin(value)));
}

template<units::unit_of_category_type<units::category::angle> unit_ = units::radians>
constexpr auto acos(const floating_type value) {
    return units::convert<unit_>(units::radians(std::acos(value)));
}

template<units::unit_of_category_type<units::category::angle> unit_ = units::radians>
constexpr auto atan(const floating_type value) {
    return units::convert<unit_>(units::radians(std::atan(value)));
}

}
