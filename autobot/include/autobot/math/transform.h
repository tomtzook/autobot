#pragma once

#include "autobot/units.h"
#include "autobot/math/common.h"

namespace autobot::math  {

template<units::unit_or_measure_type unit_, size_t size_, typename derived_>
struct base_wrapped_vector {
    using unit = units_actual(unit_);
    using raw_type = unit::type;
    using type = units::measure<unit>;
    using vector = vectorf<size_>;

    constexpr base_wrapped_vector() : m_data(vector::Identity()) {}
    explicit constexpr base_wrapped_vector(const vector& data) : m_data(data) {}
    constexpr base_wrapped_vector(const base_wrapped_vector&) = default;
    constexpr base_wrapped_vector(base_wrapped_vector&&) = default;

    constexpr base_wrapped_vector& operator=(const vector& rhs) { m_data = rhs; return *this; }
    constexpr base_wrapped_vector& operator=(const base_wrapped_vector&) = default;
    constexpr base_wrapped_vector& operator=(base_wrapped_vector&&) = default;

    constexpr bool operator==(const base_wrapped_vector& rhs) const { return m_data == rhs.m_data; }
    constexpr bool operator!=(const base_wrapped_vector& rhs) const { return m_data != rhs.m_data; }

    derived_ operator-() const { return derived_{-m_data}; }

    constexpr derived_ operator+(const base_wrapped_vector& rhs) const { return derived_{m_data + rhs.m_data}; }
    constexpr derived_ operator-(const base_wrapped_vector& rhs) const { return derived_{m_data - rhs.m_data}; }
    constexpr derived_ operator+(const type rhs) const { return derived_{m_data + rhs.value()}; }
    constexpr derived_ operator-(const type rhs) const { return derived_{m_data - rhs.value()}; }
    constexpr derived_ operator*(const type rhs) const { return derived_{m_data * rhs.value()}; }
    constexpr derived_ operator/(const type rhs) const { return derived_{m_data / rhs.value()}; }

    constexpr derived_& operator+=(const base_wrapped_vector& rhs) { m_data += rhs.m_data; return static_cast<derived_&>(*this); }
    constexpr derived_& operator-=(const base_wrapped_vector& rhs) { m_data -= rhs.m_data; return static_cast<derived_&>(*this); }
    constexpr derived_& operator+=(const type rhs) { m_data += rhs.value(); return static_cast<derived_&>(*this); }
    constexpr derived_& operator-=(const type rhs) { m_data -= rhs.value(); return static_cast<derived_&>(*this); }
    constexpr derived_& operator*=(const type rhs) { m_data *= rhs.value(); return static_cast<derived_&>(*this); }
    constexpr derived_& operator/=(const type rhs) { m_data /= rhs.value(); return static_cast<derived_&>(*this); }

    [[nodiscard]] const vector& raw() const { return m_data;}
    [[nodiscard]] vector& raw() { return m_data;}

protected:
    vector m_data;
};

template<units::unit_or_measure_type unit_>
struct base_linear3 : public base_wrapped_vector<unit_, 3, base_linear3<unit_>> {
    using base = base_wrapped_vector<unit_, 3, base_linear3<unit_>>;
    using unit = base::unit;
    using raw_type = base::raw_type;
    using type = base::type;
    using vector = base::vector;

    explicit constexpr base_linear3() : base() {}
    constexpr base_linear3(const type x, const type y, const type z) : base({x.value(), y.value(), z.value()}) {}
    explicit constexpr base_linear3(const vector& data) : base(data) {}
    constexpr base_linear3& operator=(const vector& rhs) { base::operator=(rhs); return *this; }

    constexpr base_linear3(const base_linear3&) = default;
    constexpr base_linear3(base_linear3&&) = default;
    constexpr base_linear3& operator=(const base_linear3&) = default;
    constexpr base_linear3& operator=(base_linear3&&) = default;

    [[nodiscard]] constexpr type x() const { return type(this->m_data[0]); }
    [[nodiscard]] constexpr type y() const { return type(this->m_data[1]); }
    [[nodiscard]] constexpr type z() const { return type(this->m_data[2]); }
    constexpr void x(const type value) const { this->m_data[0] = value.value(); }
    constexpr void y(const type value) const { this->m_data[1] = value.value(); }
    constexpr void z(const type value) const { this->m_data[2] = value.value(); }
};

template<units::unit_or_measure_type unit_>
struct base_angular3 : public base_wrapped_vector<unit_, 3, base_angular3<unit_>> {
    using base = base_wrapped_vector<unit_, 3, base_angular3<unit_>>;
    using unit = base::unit;
    using raw_type = base::raw_type;
    using type = base::type;
    using vector = base::vector;

    explicit constexpr base_angular3() : base() {}
    constexpr base_angular3(const type roll, const type pitch, const type yaw) : base({roll.value(), pitch.value(), yaw.value()}) {}
    explicit constexpr base_angular3(const vector& data) : base(data) {}
    constexpr base_angular3& operator=(const vector& rhs) { base::operator=(rhs); return *this; }

    constexpr base_angular3(const base_angular3&) = default;
    constexpr base_angular3(base_angular3&&) = default;
    constexpr base_angular3& operator=(const base_angular3&) = default;
    constexpr base_angular3& operator=(base_angular3&&) = default;

    constexpr type roll() const { return type(this->m_data[0]); }
    constexpr type pitch() const { return type(this->m_data[1]); }
    constexpr type yaw() const { return type(this->m_data[2]); }
    constexpr void roll(const type value) const { this->m_data[0] = value.value(); }
    constexpr void pitch(const type value) const { this->m_data[1] = value.value(); }
    constexpr void yaw(const type value) const { this->m_data[2] = value.value(); }
};

template<units::unit_of_category_type<units::category::angle> unit_>
struct base_rotation3 {
    using unit = units_actual(unit_);
    using raw_type = unit::type;
    using type = units::measure<unit>;
    using quaternion = quaternionf;
    using rotation_mat = matrix3f;
    using angular_pos = base_angular3<unit_>;

    explicit constexpr base_rotation3() : m_data(quaternion::Identity()) {}
    constexpr base_rotation3(const type roll, const type pitch, const type yaw) : m_data(from_euler_angles(roll.value(), pitch.value(), yaw.value())) {}
    explicit constexpr base_rotation3(const rotation_mat& data) : m_data(quaternion(data)) {}
    explicit constexpr base_rotation3(const quaternion& data) : m_data(data) {}
    explicit constexpr base_rotation3(const angular_pos& data) : m_data(from_euler_angles(data.roll().value(), data.pitch().value(), data.yaw().value())) {}
    constexpr base_rotation3(const base_rotation3&) = default;
    constexpr base_rotation3(base_rotation3&&) = default;

    constexpr base_rotation3& operator=(const rotation_mat& rhs) { m_data = quaternion(rhs); return *this; }
    constexpr base_rotation3& operator=(const quaternion& rhs) { m_data = rhs; return *this; }
    constexpr base_rotation3& operator=(const angular_pos& rhs) { m_data = from_euler_angles(rhs.roll().value(), rhs.pitch().value(), rhs.yaw().value()); return *this; }
    constexpr base_rotation3& operator=(const base_rotation3&) = default;
    constexpr base_rotation3& operator=(base_rotation3&&) = default;

    constexpr base_rotation3 operator+(const base_rotation3& rhs) const { return {m_data * rhs.m_data}; }
    constexpr base_rotation3 operator-(const base_rotation3& rhs) const { return {m_data.inverse() * rhs.m_data}; }
    constexpr base_rotation3 operator+(const type rhs) const { return {m_data + rhs.value()}; }
    constexpr base_rotation3 operator-(const type rhs) const { return {m_data - rhs.value()}; }
    constexpr base_rotation3 operator*(const type rhs) const { return {m_data * rhs.value()}; }
    constexpr base_rotation3 operator/(const type rhs) const { return {m_data / rhs.value()}; }

    template<units::unit_of_category_type<units::category::length> unit2_>
    constexpr base_linear3<unit2_> operator*(const base_linear3<unit2_>& rhs) const { return base_linear3<unit2_>{m_data * rhs.raw()}; }

    constexpr base_rotation3& operator+=(const base_rotation3& rhs) { m_data *= rhs.m_data; return *this; }
    constexpr base_rotation3& operator-=(const base_rotation3& rhs) { m_data = m_data.inverse() * rhs.m_data; return *this; }
    constexpr base_rotation3& operator+=(const type rhs) { m_data += rhs.value(); return *this; }
    constexpr base_rotation3& operator-=(const type rhs) { m_data -= rhs.value(); return *this; }
    constexpr base_rotation3& operator*=(const type rhs) { m_data *= rhs.value(); return *this; }
    constexpr base_rotation3& operator/=(const type rhs) { m_data /= rhs.value(); return *this; }

    [[nodiscard]] const quaternion& quat() const { return m_data; }
    [[nodiscard]] quaternion& quat() { return m_data; }

    [[nodiscard]] rotation_mat mat() const { return m_data.toRotationMatrix(); }
    [[nodiscard]] angular_pos euler() const { return angular_pos(m_data.toRotationMatrix().eulerAngles(0, 1, 2)); }

    [[nodiscard]] type roll() const { return units::convert<type>(roll_rad()); }
    [[nodiscard]] type pitch() const { return units::convert<type>(pitch_rad()); }
    [[nodiscard]] type yaw() const { return units::convert<type>(yaw_rad()); }

private:
    static quaternion from_euler_angles(const raw_type roll, const raw_type pitch, const raw_type yaw) {
        return Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
            Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()) *
            Eigen::AngleAxisd(roll,  Eigen::Vector3d::UnitX());
    }
    [[nodiscard]] units::radians roll_rad() const {
        const auto w = m_data.w();
        const auto x = m_data.x();
        const auto y = m_data.y();
        const auto z = m_data.z();

        const auto cxcy = 1.0 - 2.0 * (x * x + y * y);
        const auto sxcy = 2.0 * (w * x + y * z);
        const auto cy_sq = cxcy * cxcy + sxcy * sxcy;
        if (cy_sq > 1e-20) {
            return units::radians(atan2(sxcy, cxcy));
        }

        return units::radians(0);
    }
    [[nodiscard]] units::radians pitch_rad() const {
        const auto w = m_data.w();
        const auto x = m_data.x();
        const auto y = m_data.y();
        const auto z = m_data.z();

        const auto ratio = 2.0 * (w * y - z * x);
        if (abs(ratio) >= 1.0) {
            const auto val = pi / 2.0 * (ratio >= 0 ? 1 : -1);
            return units::radians(val);
        }

        return units::radians(asin(ratio));
    }
    [[nodiscard]] units::radians yaw_rad() const {
        const auto w = m_data.w();
        const auto x = m_data.x();
        const auto y = m_data.y();
        const auto z = m_data.z();

        const auto cycz = 1.0 - 2.0 * (y * y + z * z);
        const auto cysz = 2.0 * (w * z + x * y);
        const auto cy_sq = cycz * cycz + cysz * cysz;
        if (cy_sq > 1e-20) {
            return units::radians(atan2(cysz, cycz));
        }

        return units::radians(atan2(2.0 * w * z, w * w - z * z));
    }

    quaternion m_data;
};

template<units::unit_of_category_type<units::category::length> unit_trans_, units::unit_of_category_type<units::category::angle> unit_rot_>
struct base_transform3 {
    using translation_type = base_linear3<unit_trans_>;
    using rotation_type = base_rotation3<unit_rot_>;
    using translation_underlying_type = translation_type::type;
    using rotation_underlying_type = rotation_type::type;
    using isometry = Eigen::Transform<floating_type, 3, Eigen::Isometry>;

    explicit constexpr base_transform3() : m_data(isometry::Identity()) {}
    explicit constexpr base_transform3(const isometry& data) : m_data(data) {}
    explicit constexpr base_transform3(const translation_type& translation, const rotation_type& rotation) : m_data(create(translation, rotation)) {}
    explicit constexpr base_transform3(
        const translation_underlying_type x, const translation_underlying_type y, const translation_underlying_type z,
        const rotation_underlying_type roll, const rotation_underlying_type pitch, const rotation_underlying_type yaw) : base_transform3(translation_type{x,y,z}, rotation_type{roll,pitch,yaw}) {}
    constexpr base_transform3(const base_transform3&) = default;
    constexpr base_transform3(base_transform3&&) = default;

    constexpr base_transform3& operator=(const isometry& rhs) { m_data = rhs; return *this; }
    constexpr base_transform3& operator=(const base_transform3&) = default;
    constexpr base_transform3& operator=(base_transform3&&) = default;

    translation_type translation() const { return translation_type{m_data.translation()}; }
    void translation(const translation_type& value) const { m_data.translation() = value.raw(); }

    rotation_type rotation() const { return rotation_type{m_data.linear()}; }
    void rotation(const rotation_type& rotation) { m_data.linear() = rotation.mat(); }

    [[nodiscard]] const isometry& raw() const { return m_data; }
    [[nodiscard]] isometry& raw() { return m_data; }

private:
    static isometry create(const translation_type& translation, const rotation_type& rotation) {
        isometry data;
        data.translation() = translation.raw();
        data.linear() = rotation.mat();
        return data;
    }

    isometry m_data;
};

template<units::unit_of_category_type<units::category::length> unit_>
using linear_position3 = base_linear3<unit_>;
template<units::unit_of_category_type<units::category::linear_velocity> unit_>
using linear_velocity3 = base_linear3<unit_>;
template<units::unit_of_category_type<units::category::linear_acceleration> unit_>
using linear_acceleration3 = base_linear3<unit_>;

template<units::unit_of_category_type<units::category::angle> unit_>
using angular_position3 = base_angular3<unit_>; // euler angles
template<units::unit_of_category_type<units::category::angular_velocity> unit_>
using angular_velocity3 = base_angular3<unit_>;
template<units::unit_of_category_type<units::category::angular_acceleration> unit_>
using angular_acceleration3 = base_angular3<unit_>;

using translation3 = base_linear3<units::meters>;
using rotation3 = base_rotation3<units::radians>;
using axis3 = base_linear3<units::meters>;
using transform3 = base_transform3<units::meters, units::radians>;

constexpr axis3 axis_x() {
    return axis3{Eigen::Vector3d::UnitX()};
}

constexpr axis3 axis_y() {
    return axis3{Eigen::Vector3d::UnitY()};
}

constexpr axis3 axis_z() {
    return axis3{Eigen::Vector3d::UnitZ()};
}

constexpr axis3 forward() {
    return axis3{Eigen::Vector3d::UnitX()};
}

constexpr axis3 backward() {
    return axis3{-Eigen::Vector3d::UnitX()};
}

constexpr axis3 left() {
    return axis3{Eigen::Vector3d::UnitY()};
}

constexpr axis3 right() {
    return axis3{-Eigen::Vector3d::UnitY()};
}

constexpr axis3 up() {
    return axis3{Eigen::Vector3d::UnitZ()};
}

constexpr axis3 down() {
    return axis3{-Eigen::Vector3d::UnitZ()};
}

}
