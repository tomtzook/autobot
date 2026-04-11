#pragma once

#include <units.h>

namespace autobot::sim::dynamics {

enum class ligament_aspect {
    none = 0,
    visual = 1,
    collision = 2,
    dynamics = 4,
    all = 7
};

constexpr ligament_aspect operator|(const ligament_aspect lhs, const ligament_aspect rhs) {
    return static_cast<ligament_aspect>(static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs));
}

constexpr ligament_aspect operator&(const ligament_aspect lhs, const ligament_aspect rhs) {
    return static_cast<ligament_aspect>(static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs));
}

constexpr bool operator==(const ligament_aspect lhs, const ligament_aspect rhs) {
    return static_cast<uint16_t>(lhs) == static_cast<uint16_t>(rhs);
}

constexpr bool operator!=(const ligament_aspect lhs, const ligament_aspect rhs) {
    return static_cast<uint16_t>(lhs) != static_cast<uint16_t>(rhs);
}

struct empty_shape {
};

struct box_shape {
    constexpr box_shape(units::meters length, units::meters width, units::meters height);

    [[nodiscard]] constexpr Eigen::Vector3d dims() const;

    units::meters length;
    units::meters width;
    units::meters height;
};

struct sphere_shape {
    explicit constexpr sphere_shape(units::meters radius);

    units::meters radius;
};

struct cylinder_shape {
    constexpr cylinder_shape(units::meters radius, units::meters height);

    units::meters radius;
    units::meters height;
};

struct ellipsoid_shape {
    constexpr ellipsoid_shape(units::meters diameter_x, units::meters diameter_y, units::meters diameter_z);

    [[nodiscard]] constexpr Eigen::Vector3d dims() const;

    units::meters diameter_x;
    units::meters diameter_y;
    units::meters diameter_z;
};

template<typename t_>
concept shape_type = std::is_same_v<t_, empty_shape> || std::is_same_v<t_, box_shape> || std::is_same_v<t_, sphere_shape> ||
    std::is_same_v<t_, cylinder_shape> || std::is_same_v<t_, ellipsoid_shape>;

struct weld_joint {

};

struct revolute_joint {
    Eigen::Vector3d rotation_axis;
};

struct prismatic_joint {

};

struct ball_joint {

};

struct free_joint {

};

template<typename t_>
concept joint_type = std::is_same_v<t_, weld_joint> || std::is_same_v<t_, revolute_joint> ||
    std::is_same_v<t_, prismatic_joint> || std::is_same_v<t_, ball_joint> || std::is_same_v<t_, free_joint>;

struct raycast_result {
    units::meters distance;
    Eigen::Vector3d impact_point;
};

constexpr box_shape::box_shape(const units::meters length, const units::meters width, const units::meters height)
    : length(length)
    , width(width)
    , height(height)
{}

constexpr Eigen::Vector3d box_shape::dims() const {
    return {length.value(), width.value(), height.value()};
}

constexpr sphere_shape::sphere_shape(const units::meters radius)
    : radius(radius)
{}

constexpr cylinder_shape::cylinder_shape(const units::meters radius, const units::meters height)
    : radius(radius)
    , height(height)
{}


constexpr ellipsoid_shape::ellipsoid_shape(const units::meters diameter_x, const units::meters diameter_y, const units::meters diameter_z)
    : diameter_x(diameter_x)
    , diameter_y(diameter_y)
    , diameter_z(diameter_z)
{}

constexpr Eigen::Vector3d ellipsoid_shape::dims() const {
    return Eigen::Vector3d{diameter_x.value(), diameter_y.value(), diameter_z.value()};
}

}
