#pragma once

#include "autobot_sim/dynamics/engine.h"

namespace autobot::sim::dynamics {

template<typename joint_t_>
class ligament;

class body_node {
public:
    body_node(engine::body_holder body_holder, engine::body_node_holder holder);

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

    [[nodiscard]] std::optional<raycast_result> raycast(const Eigen::Vector3d& origin_offset, const Eigen::Vector3d& direction, units::meters max_distance) const;
    [[nodiscard]] std::optional<raycast_result> raycast(const Eigen::Vector3d& origin_offset, const Eigen::Vector3d& direction, units::meters min_distance, units::meters max_distance) const;

private:
    engine::body_holder m_body_holder;
    engine::body_node_holder m_holder;

    template<typename joint_t_> friend class ligament;
};

template<joint_type>
class joint{};

template<>
class joint<weld_joint> {
public:
    using joint_info = engine::joint_info<weld_joint>;

    explicit joint(engine::joint_holder<weld_joint> joint);

private:
    engine::joint_holder<weld_joint> m_joint;

    friend class ligament<weld_joint>;
};

template<>
class joint<revolute_joint> {
public:
    using joint_info = engine::joint_info<revolute_joint>;

    explicit joint(engine::joint_holder<revolute_joint> joint);

    [[nodiscard]] units::radians get_position() const;
    void set_position(units::radians value);
    [[nodiscard]] units::radians_per_second get_velocity() const;
    void set_velocity(units::radians_per_second value);
    [[nodiscard]] units::radians_per_second_squared get_acceleration() const;
    void set_acceleration(units::radians_per_second_squared value);

private:
    engine::joint_holder<revolute_joint> m_joint;

    friend class ligament<revolute_joint>;
};

template<>
class joint<prismatic_joint> {
public:
    using joint_info = engine::joint_info<prismatic_joint>;

    explicit joint(engine::joint_holder<prismatic_joint> joint);

    [[nodiscard]] units::meters get_position() const;
    void set_position(units::meters value);
    [[nodiscard]] units::meters_per_second get_velocity() const;
    void set_velocity(units::meters_per_second value);
    [[nodiscard]] units::meters_per_second_squared get_acceleration() const;
    void set_acceleration(units::meters_per_second_squared value);

private:
    engine::joint_holder<prismatic_joint> m_joint;

    friend class ligament<prismatic_joint>;
};

template<>
class joint<ball_joint> {
public:
    using joint_info = engine::joint_info<ball_joint>;

    explicit joint(engine::joint_holder<ball_joint> joint);

    [[nodiscard]] Eigen::Vector3<units::radians> get_position() const;
    void set_position(const Eigen::Vector3<units::radians>& value);
    [[nodiscard]] Eigen::Vector3<units::radians_per_second> get_velocity() const;
    void set_velocity(const Eigen::Vector3<units::radians_per_second>& value);
    [[nodiscard]] Eigen::Vector3<units::radians_per_second_squared> get_acceleration() const;
    void set_acceleration(const Eigen::Vector3<units::radians_per_second_squared>& value);

private:
    engine::joint_holder<ball_joint> m_joint;

    friend class ligament<ball_joint>;
};

template<>
class joint<free_joint> {
public:
    using joint_info = engine::joint_info<free_joint>;

    explicit joint(engine::joint_holder<free_joint> joint);

    [[nodiscard]] Eigen::Vector3<units::meters> get_linear_position() const;
    void set_linear_position(const Eigen::Vector3<units::meters>& value);
    [[nodiscard]] Eigen::Vector3<units::meters_per_second> get_linear_velocity() const;
    void set_linear_velocity(const Eigen::Vector3<units::meters_per_second>& value);
    [[nodiscard]] Eigen::Vector3<units::meters_per_second_squared> get_linear_acceleration() const;
    void set_linear_acceleration(const Eigen::Vector3<units::meters_per_second_squared>& value);

    [[nodiscard]] Eigen::Vector3<units::radians> get_angular_position() const;
    void set_angular_position(const Eigen::Vector3<units::radians>& value);
    [[nodiscard]] Eigen::Vector3<units::radians_per_second> get_angular_velocity() const;
    void set_angular_velocity(const Eigen::Vector3<units::radians_per_second>& value);
    [[nodiscard]] Eigen::Vector3<units::radians_per_second_squared> get_angular_acceleration() const;
    void set_angular_acceleration(const Eigen::Vector3<units::radians_per_second_squared>& value);

private:
    engine::joint_holder<free_joint> m_joint;

    friend class ligament<free_joint>;
};

template<joint_type joint_t_>
class ligament<joint_t_> {
public:
    ligament(const engine::body_holder& body_holder, engine::joint_holder<joint_t_>&& joint_holder, engine::body_node_holder&& body_node_holder);

    ligament(const ligament&) = delete;
    ligament(ligament&&) = default;
    ligament& operator=(const ligament&) = delete;
    ligament& operator=(ligament&&) = default;

    const joint<joint_t_>& get_joint() const { return m_joint; }
    joint<joint_t_>& get_joint() { return m_joint; }
    const body_node& get_node() const { return m_body_node; }
    body_node& get_node() { return m_body_node; }

    template<shape_type shape_t_, joint_type joint_t2_>
    [[nodiscard]] ligament<joint_t2_> attach(std::string_view name,
        const shape_t_& shape, const joint_t2_& joint,
        const Eigen::Isometry3d& transform,
        ligament_aspect aspects = ligament_aspect::none);

private:
    engine::body_holder m_body_holder;
    joint<joint_t_> m_joint;
    body_node m_body_node;

    friend class container;
};

class container : public body_node {
public:
    explicit container(engine::body_holder&& body);

    template<shape_type shape_t_, joint_type joint_t_>
    ligament<joint_t_> attach(std::string_view name,
        const shape_t_& shape, const joint_t_& joint,
        const Eigen::Isometry3d& transform,
        ligament_aspect aspects = ligament_aspect::none);

private:
    container(const engine::body_holder& body, ligament<free_joint>&& root);

    static ligament<free_joint> create_root(const engine::body_holder& body);

    engine::body_holder m_holder;
    ligament<free_joint> m_root;

    friend class world;
};

class world {
public:
    world();
    world(const world&) = delete;
    world(world&&) = default;
    world& operator=(const world&) = delete;
    world& operator=(world&&) = default;

    container create(std::string_view name);

    [[nodiscard]] std::optional<raycast_result> raycast(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, units::meters max_distance) const;
    [[nodiscard]] std::optional<raycast_result> raycast(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, units::meters min_distance, units::meters max_distance) const;

    void step();
    void render(engine::render_function&& render_action);

private:
    engine::world_holder m_holder;
};

template<joint_type joint_t_>
ligament<joint_t_>::ligament(const engine::body_holder& body_holder, engine::joint_holder<joint_t_>&& joint_holder, engine::body_node_holder&& body_node_holder)
    : m_body_holder(body_holder)
    , m_joint(std::move(joint_holder))
    , m_body_node(m_body_holder, std::move(body_node_holder))
{}

template<joint_type joint_t_>
template<shape_type shape_t_, joint_type joint_t2_>
ligament<joint_t2_> ligament<joint_t_>::attach(
    std::string_view name,
    const shape_t_& shape, const joint_t2_& joint,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    auto [joint_holder, body_node_holder] = create_ligament(m_body_holder, m_body_node.m_holder, name, shape, joint, transform, aspects);
    return ligament<joint_t2_>(m_body_holder, std::move(joint_holder), std::move(body_node_holder));
}

template<shape_type shape_t_, joint_type joint_t_>
ligament<joint_t_> container::attach(
    const std::string_view name,
    const shape_t_& shape, const joint_t_& joint,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    return m_root.attach(name, shape, joint, transform, aspects);
}

}