#pragma once

#include <cmath>
#include <Eigen/Core>

namespace autobot::math {

template<typename t_>
concept numeric = std::integral<t_> || std::floating_point<t_>;

using integer_type = int;
using floating_type = float;

constexpr floating_type pi = M_PIf;

template<size_t rows, size_t cols>
using matrixd = Eigen::Matrix<integer_type, rows, cols>;
template<size_t rows, size_t cols>
using matrixf = Eigen::Matrix<floating_type, rows, cols>;

using vector2d = Eigen::Vector2<integer_type>;
using vector2f = Eigen::Vector2<floating_type>;
using matrix2d = Eigen::Matrix2<integer_type>;
using matrix2f = Eigen::Matrix2<floating_type>;
using vector3d = Eigen::Vector3<integer_type>;
using vector3f = Eigen::Vector3<floating_type>;
using matrix3d = Eigen::Matrix3<integer_type>;
using matrix3f = Eigen::Matrix3<floating_type>;


template<std::floating_point type_ = floating_type>
constexpr type_ translate_angle(const type_ deg) {
    auto corrected = std::fmod(deg, 360.0f);
    if (corrected < 0) {
        corrected += 360;
    }

    return corrected;
}

template<numeric type_>
constexpr type_ constrain(const type_ value, const type_ min, const type_ max) {
    if (value > max) {
        return max;
    }
    if (value < min) {
        return min;
    }

    return value;
}

template<std::integral type_ = integer_type>
constexpr type_ round_up(const type_ num, const type_ multiple) {
    if (multiple == 0)
        return num;

    const auto remainder = num % multiple;
    if (remainder == 0)
        return num;

    return num + multiple - remainder;
}

template<std::integral type_ = integer_type>
constexpr type_ round_down(const type_ num, const type_ multiple) {
    if (multiple == 0)
        return num;

    const auto remainder = num % multiple;
    if (remainder == 0)
        return num;

    return num - remainder;
}

}
