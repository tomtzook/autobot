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
    box_shape(units::meters length, units::meters width, units::meters height);

    [[nodiscard]] Eigen::Vector3d dims() const;

    units::meters length;
    units::meters width;
    units::meters height;
};

struct sphere_shape {
    explicit sphere_shape(units::meters radius);

    units::meters radius;
};

struct cylinder_shape {
    cylinder_shape(units::meters radius, units::meters height);

    units::meters radius;
    units::meters height;
};

struct ellipsoid_shape {
    ellipsoid_shape(units::meters diameter_x, units::meters diameter_y, units::meters diameter_z);

    [[nodiscard]] Eigen::Vector3d dims() const;

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

}
