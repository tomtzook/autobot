#pragma once

#include "autobot_sim/dynamics/engine/world.h"

namespace autobot::sim::dynamics::engine {

struct ligament_holder {
    ligament_holder() = default;
    virtual ~ligament_holder() = default;
};

struct body_holder {
    body_holder() = default;
    virtual ~body_holder() = default;
};

using generic_shape = std::variant<empty_shape, box_shape, sphere_shape, cylinder_shape, ellipsoid_shape>;
using ligament_holder_ptr = std::shared_ptr<ligament_holder>;
using body_holder_ptr = std::shared_ptr<body_holder>;

ligament_holder_ptr create_root_ligament(
    const body_holder_ptr& body,
    std::string_view name,
    ligament_aspect aspects);
ligament_holder_ptr create_ligament(
    const ligament_holder_ptr& parent,
    std::string_view name,
    const generic_shape& shape_info,
    const weld_joint& joint_info,
    const Eigen::Isometry3d& transform,
    ligament_aspect aspects);
ligament_holder_ptr create_ligament(
    const ligament_holder_ptr& parent,
    std::string_view name,
    const generic_shape& shape_info,
    const revolute_joint& joint_info,
    const Eigen::Isometry3d& transform,
    ligament_aspect aspects);
ligament_holder_ptr create_ligament(
    const ligament_holder_ptr& parent,
    std::string_view name,
    const generic_shape& shape_info,
    const prismatic_joint& joint_info,
    const Eigen::Isometry3d& transform,
    ligament_aspect aspects);
ligament_holder_ptr create_ligament(
    const ligament_holder_ptr& parent,
    std::string_view name,
    const generic_shape& shape_info,
    const ball_joint& joint_info,
    const Eigen::Isometry3d& transform,
    ligament_aspect aspects);
ligament_holder_ptr create_ligament(
    const ligament_holder_ptr& parent,
    std::string_view name,
    const generic_shape& shape_info,
    const free_joint& joint_info,
    const Eigen::Isometry3d& transform,
    ligament_aspect aspects);

body_holder_ptr create_body(const world_holder_ptr& world, std::string_view name);
world_holder_ptr get_world(const ligament_holder_ptr& ptr);

struct ligament_info {
    static Eigen::Isometry3d get_world_transform(const ligament_holder_ptr& ptr);
    static Eigen::Vector3d get_world_position(const ligament_holder_ptr& ptr);
    static Eigen::Matrix3d get_world_rotation(const ligament_holder_ptr& ptr);
    static Eigen::Vector3d get_world_linear_velocity(const ligament_holder_ptr& ptr);
    static Eigen::Vector3d get_world_angular_velocity(const ligament_holder_ptr& ptr);
    static Eigen::Vector3d get_world_linear_acceleration(const ligament_holder_ptr& ptr);
    static Eigen::Vector3d get_world_angular_acceleration(const ligament_holder_ptr& ptr);
};

template<joint_type joint_t_>
struct joint_info {};

template<>
struct joint_info<weld_joint> {};

template<>
struct joint_info<revolute_joint> {
    using vector = Eigen::Vector<double, 1>;

    static vector get_position(const ligament_holder_ptr& ptr);
    static void set_position(const ligament_holder_ptr& ptr, const vector& value);
    static vector get_velocity(const ligament_holder_ptr& ptr);
    static void set_velocity(const ligament_holder_ptr& ptr, const vector& value);
    static vector get_acceleration(const ligament_holder_ptr& ptr);
    static void set_acceleration(const ligament_holder_ptr& ptr, const vector& value);
    static void set_command(const ligament_holder_ptr& ptr, const vector& value);
};

template<>
struct joint_info<prismatic_joint> {
    using vector = Eigen::Vector<double, 1>;

    static vector get_position(const ligament_holder_ptr& ptr);
    static void set_position(const ligament_holder_ptr& ptr, const vector& value);
    static vector get_velocity(const ligament_holder_ptr& ptr);
    static void set_velocity(const ligament_holder_ptr& ptr, const vector& value);
    static vector get_acceleration(const ligament_holder_ptr& ptr);
    static void set_acceleration(const ligament_holder_ptr& ptr, const vector& value);
    static void set_command(const ligament_holder_ptr& ptr, const vector& value);
};

template<>
struct joint_info<ball_joint> {
    using vector = Eigen::Vector<double, 3>;

    static vector get_position(const ligament_holder_ptr& ptr);
    static void set_position(const ligament_holder_ptr& ptr, const vector& value);
    static vector get_velocity(const ligament_holder_ptr& ptr);
    static void set_velocity(const ligament_holder_ptr& ptr, const vector& value);
    static vector get_acceleration(const ligament_holder_ptr& ptr);
    static void set_acceleration(const ligament_holder_ptr& ptr, const vector& value);
    static void set_command(const ligament_holder_ptr& ptr, const vector& value);
};

template<>
struct joint_info<free_joint> {
    using vector = Eigen::Vector<double, 6>;

    static vector get_position(const ligament_holder_ptr& ptr);
    static void set_position(const ligament_holder_ptr& ptr, const vector& value);
    static vector get_velocity(const ligament_holder_ptr& ptr);
    static void set_velocity(const ligament_holder_ptr& ptr, const vector& value);
    static vector get_acceleration(const ligament_holder_ptr& ptr);
    static void set_acceleration(const ligament_holder_ptr& ptr, const vector& value);
    static void set_command(const ligament_holder_ptr& ptr, const vector& value);
};


}
