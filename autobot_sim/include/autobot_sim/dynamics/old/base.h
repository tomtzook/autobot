#pragma once

#include <utility>

#include "base.h"
#include "autobot_sim/dynamics/engine.h"

namespace autobot::sim::dynamics::engine {

template<typename joint_t_>
struct ligament;

struct body_node {
    body_node(const world_holder& world, body_node_holder holder);

    [[nodiscard]] Eigen::Isometry3d get_world_transform() const;
    [[nodiscard]] Eigen::Vector3d get_world_position() const;
    [[nodiscard]] Eigen::Quaterniond get_world_rotation() const;
    [[nodiscard]] Eigen::Vector3d get_world_linear_velocity() const;
    [[nodiscard]] Eigen::Vector3d get_world_angular_velocity() const;
    [[nodiscard]] Eigen::Vector3d get_world_linear_acceleration() const;
    [[nodiscard]] Eigen::Vector3d get_world_angular_acceleration() const;

    [[nodiscard]] Eigen::Vector3d forward() const;
    [[nodiscard]] Eigen::Vector3d backward() const;
    [[nodiscard]] Eigen::Vector3d left() const;
    [[nodiscard]] Eigen::Vector3d right() const;
    [[nodiscard]] Eigen::Vector3d up() const;
    [[nodiscard]] Eigen::Vector3d down() const;

    const world_holder& world;
    body_node_holder holder;
};

template<joint_type>
struct joint{};

template<>
struct joint<weld_joint> {
    using joint_info = joint_info<weld_joint>;

    explicit joint(joint_holder<weld_joint> holder);

    joint_holder<weld_joint> holder;
};

template<>
struct joint<revolute_joint> {
    using joint_info = joint_info<revolute_joint>;

    explicit joint(joint_holder<revolute_joint> holder);

    [[nodiscard]] double get_position() const;
    void set_position(double value);
    [[nodiscard]] double get_velocity() const;
    void set_velocity(double value);
    [[nodiscard]] double get_acceleration() const;
    void set_acceleration(double value);

    joint_holder<revolute_joint> holder;
};

template<>
struct joint<prismatic_joint> {
    using joint_info = joint_info<prismatic_joint>;

    explicit joint(joint_holder<prismatic_joint> holder);

    [[nodiscard]] double get_position() const;
    void set_position(double value);
    [[nodiscard]] double get_velocity() const;
    void set_velocity(double value);
    [[nodiscard]] double get_acceleration() const;
    void set_acceleration(double value);

    joint_holder<prismatic_joint> holder;
};

template<>
struct joint<ball_joint> {
    using joint_info = joint_info<ball_joint>;

    explicit joint(joint_holder<ball_joint> holder);

    [[nodiscard]] Eigen::Vector3d get_position() const;
    void set_position(const Eigen::Vector3d& value);
    [[nodiscard]] Eigen::Vector3d get_velocity() const;
    void set_velocity(const Eigen::Vector3d& value);
    [[nodiscard]] Eigen::Vector3d get_acceleration() const;
    void set_acceleration(const Eigen::Vector3d& value);

    joint_holder<ball_joint> holder;
};

template<>
struct joint<free_joint> {
    using joint_info = joint_info<free_joint>;

    explicit joint(joint_holder<free_joint> holder);

    [[nodiscard]] Eigen::Vector3d get_linear_position() const;
    void set_linear_position(const Eigen::Vector3d& value);
    [[nodiscard]] Eigen::Vector3d get_linear_velocity() const;
    void set_linear_velocity(const Eigen::Vector3d& value);
    [[nodiscard]] Eigen::Vector3d get_linear_acceleration() const;
    void set_linear_acceleration(const Eigen::Vector3d& value);

    [[nodiscard]] Eigen::Vector3d get_angular_position() const;
    void set_angular_position(const Eigen::Vector3d& value);
    [[nodiscard]] Eigen::Vector3d get_angular_velocity() const;
    void set_angular_velocity(const Eigen::Vector3d& value);
    [[nodiscard]] Eigen::Vector3d get_angular_acceleration() const;
    void set_angular_acceleration(const Eigen::Vector3d& value);

    joint_holder<free_joint> holder;
};

template<joint_type joint_t_>
struct ligament<joint_t_> {
    ligament(const world_holder& world, body_holder body_holder, joint_holder<joint_t_>&& joint_holder, body_node_holder&& body_node_holder);

    template<shape_type shape_t_, joint_type joint_t2_>
    ligament<joint_t2_> attach(
        std::string_view name,
        const shape_t_& shape_info, const joint_t2_& joint_info,
        const Eigen::Isometry3d& transform,
        ligament_aspect aspects);

    const world_holder& world;
    body_holder body;
    joint<joint_t_> joint;
    body_node node;
};

struct body {
    body(const world_holder& world, body_holder&& body);
    body(const world_holder& world, body_holder&& body, ligament<free_joint>&& root);

    const world_holder& world;
    body_holder holder;
    ligament<free_joint> root;
};

struct world {
    world();

    body create(std::string_view name);

    void step();
    void render(render_function&& render_action);

    world_holder holder;
};

template<joint_type joint_t_>
ligament<joint_t_>::ligament(const world_holder& world, body_holder body_holder, joint_holder<joint_t_>&& joint_holder, body_node_holder&& body_node_holder)
    : world(world)
    , body(std::move(body_holder))
    , joint(std::move(joint_holder))
    , node(body_node_holder)
{}

template<joint_type joint_t_>
template<shape_type shape_t_, joint_type joint_t2_>
ligament<joint_t2_> ligament<joint_t_>::attach(
    const std::string_view name,
    const shape_t_& shape_info, const joint_t2_& joint_info,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    auto [joint_holder, body_node_holder] = create_ligament(world, body, node.holder, name, shape_info, joint_info, transform, aspects);
    return ligament<joint_t2_>(body, std::move(joint_holder), std::move(body_node_holder));
}

std::optional<raycast_result> raycast( const world_holder& world, const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, units::meters max_distance);
std::optional<raycast_result> raycast(const world_holder& world, const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, units::meters min_distance, units::meters max_distance);

}