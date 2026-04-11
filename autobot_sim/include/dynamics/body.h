#pragma once

#include "dynamics/engine.h"

namespace autobot::sim::dynamics {

template<joint_type joint_t_>
class world_transformable {
public:
    explicit world_transformable(engine::ligament_holder<joint_t_> ligament);

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

protected:
    engine::ligament_holder<joint_t_> m_ligament;
};

template<joint_type>
class transformable{};

template<>
class transformable<weld_joint> : public world_transformable<weld_joint> {
public:
    using joint_info = engine::joint_info<weld_joint>;

    explicit transformable(engine::ligament_holder<weld_joint> ligament);
};

template<>
class transformable<revolute_joint> : public world_transformable<revolute_joint> {
public:
    using joint_info = engine::joint_info<revolute_joint>;

    explicit transformable(engine::ligament_holder<revolute_joint> ligament);

    [[nodiscard]] units::radians get_position() const;
    void set_position(units::radians value);
    [[nodiscard]] units::radians_per_second get_velocity() const;
    void set_velocity(units::radians_per_second value);
    [[nodiscard]] units::radians_per_second_squared get_acceleration() const;
    void set_acceleration(units::radians_per_second_squared value);
};

template<>
class transformable<prismatic_joint> : public world_transformable<prismatic_joint> {
public:
    using joint_info = engine::joint_info<prismatic_joint>;

    explicit transformable(engine::ligament_holder<prismatic_joint> ligament);

    [[nodiscard]] units::meters get_position() const;
    void set_position(units::meters value);
    [[nodiscard]] units::meters_per_second get_velocity() const;
    void set_velocity(units::meters_per_second value);
    [[nodiscard]] units::meters_per_second_squared get_acceleration() const;
    void set_acceleration(units::meters_per_second_squared value);
};

template<>
class transformable<ball_joint> : public world_transformable<ball_joint> {
public:
    using joint_info = engine::joint_info<ball_joint>;

    explicit transformable(engine::ligament_holder<ball_joint> ligament);

    [[nodiscard]] Eigen::Vector3<units::radians> get_position() const;
    void set_position(const Eigen::Vector3<units::radians>& value);
    [[nodiscard]] Eigen::Vector3<units::radians_per_second> get_velocity() const;
    void set_velocity(const Eigen::Vector3<units::radians_per_second>& value);
    [[nodiscard]] Eigen::Vector3<units::radians_per_second_squared> get_acceleration() const;
    void set_acceleration(const Eigen::Vector3<units::radians_per_second_squared>& value);
};

template<>
class transformable<free_joint> : public world_transformable<free_joint> {
public:
    using joint_info = engine::joint_info<free_joint>;

    explicit transformable(engine::ligament_holder<free_joint> ligament);

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
};

template<joint_type joint_t_>
class ligament : public transformable<joint_t_> {
public:
    ligament(const engine::body_holder& body_holder, engine::ligament_holder<joint_t_> holder);

    ligament(const ligament&) = delete;
    ligament(ligament&&) = default;
    ligament& operator=(const ligament&) = delete;
    ligament& operator=(ligament&&) = default;

    template<shape_type shape_t_, joint_type joint_t2_>
    [[nodiscard]] ligament<joint_t2_> attach(std::string_view name,
        const shape_t_& shape, const joint_t2_& joint,
        const Eigen::Isometry3d& transform,
        ligament_aspect aspects = ligament_aspect::none);
    template<shape_type shape_t_, joint_type joint_t2_, typename lig_t_, typename... args_>
    [[nodiscard]] lig_t_ attach(std::string_view name,
        const shape_t_& shape, const joint_t2_& joint,
        const Eigen::Isometry3d& transform,
        ligament_aspect aspects,
        args_... args);

    [[nodiscard]] std::optional<raycast_result> raycast(const Eigen::Vector3d& origin_offset, const Eigen::Vector3d& direction, units::meters max_distance) const;
    [[nodiscard]] std::optional<raycast_result> raycast(const Eigen::Vector3d& origin_offset, const Eigen::Vector3d& direction, units::meters min_distance, units::meters max_distance) const;

private:
    engine::body_holder m_body_holder;
    engine::ligament_holder<joint_t_> m_holder;

    friend class container;
};

class container : public transformable<free_joint> {
public:
    explicit container(engine::body_holder&& body);

    template<shape_type shape_t_, joint_type joint_t_>
    ligament<joint_t_> attach(std::string_view name,
        const shape_t_& shape, const joint_t_& joint,
        const Eigen::Isometry3d& transform,
        ligament_aspect aspects = ligament_aspect::none);

private:
    container(const engine::body_holder& body, engine::ligament_holder<free_joint>&& root);

    static engine::ligament_holder<free_joint> create_root(const engine::body_holder& body);

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

    std::optional<raycast_result> raycast(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, double max_distance) const;

    void step();
    void render(engine::render_function&& render_action);

private:
    engine::world_holder m_holder;
};

template<joint_type joint_t_>
world_transformable<joint_t_>::world_transformable(engine::ligament_holder<joint_t_> ligament)
    : m_ligament(std::move(ligament))
{}

template<joint_type joint_t_>
Eigen::Isometry3d world_transformable<joint_t_>::get_world_transform() const {
    return m_ligament.body_node->getWorldTransform();
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::get_world_position() const {
    return get_world_transform().translation();
}

template<joint_type joint_t_>
Eigen::Quaterniond world_transformable<joint_t_>::get_world_rotation() const {
    const auto transform = get_world_transform();
    auto quat = Eigen::Quaterniond(transform.linear());
    quat.normalize();
    return quat;
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::get_world_linear_velocity() const {
    return m_ligament.body_node->getLinearVelocity();
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::get_world_angular_velocity() const {
    return m_ligament.body_node->getAngularVelocity();
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::get_world_linear_acceleration() const {
    return m_ligament.body_node->getLinearAcceleration();
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::get_world_angular_acceleration() const {
    return m_ligament.body_node->getAngularAcceleration();
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::forward() const {
    const auto axis = Eigen::Vector3d::UnitX();
    return get_world_rotation() * axis;
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::backward() const {
    const auto axis = -Eigen::Vector3d::UnitX();
    return get_world_rotation() * axis;
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::left() const {
    const auto axis = Eigen::Vector3d::UnitY();
    return get_world_rotation() * axis;
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::right() const {
    const auto axis = -Eigen::Vector3d::UnitY();
    return get_world_rotation() * axis;
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::up() const {
    const auto axis = Eigen::Vector3d::UnitZ();
    return get_world_rotation() * axis;
}

template<joint_type joint_t_>
Eigen::Vector3d world_transformable<joint_t_>::down() const {
    const auto axis = -Eigen::Vector3d::UnitZ();
    return get_world_rotation() * axis;
}

template<joint_type joint_t_>
ligament<joint_t_>::ligament(const engine::body_holder& body_holder, engine::ligament_holder<joint_t_> holder)
    : transformable<joint_t_>(holder)
    , m_body_holder(body_holder)
    , m_holder(std::move(holder))
{}

template<joint_type joint_t_>
template<shape_type shape_t_, joint_type joint_t2_>
ligament<joint_t2_> ligament<joint_t_>::attach(
    std::string_view name,
    const shape_t_& shape, const joint_t2_& joint,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    auto holder = create_ligament(m_body_holder, m_holder.body_node, name, shape, joint, transform, aspects);
    return ligament<joint_t2_>(m_body_holder, holder);
}

template<joint_type joint_t_>
template<shape_type shape_t_, joint_type joint_t2_, typename lig_t_, typename... args_>
lig_t_ ligament<joint_t_>::attach(
    const std::string_view name,
    const shape_t_& shape, const joint_t2_& joint,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects,
    args_... args) {
    auto ligament = attach<shape_t_, joint_t2_>(name, shape, joint, transform, aspects);
    return lig_t_(std::move(ligament), std::forward<args_>(args)...);
}

template<joint_type joint_t_>
std::optional<raycast_result> ligament<joint_t_>::raycast(const Eigen::Vector3d& origin_offset, const Eigen::Vector3d& direction, const units::meters max_distance) const {
    return raycast(origin_offset, direction, units::meters(0), max_distance);
}

template<joint_type joint_t_>
std::optional<raycast_result> ligament<joint_t_>::raycast(const Eigen::Vector3d& origin_offset, const Eigen::Vector3d& direction, const units::meters min_distance, const units::meters max_distance) const {
    const auto origin = this->get_world_position() + origin_offset;
    return engine::raycast(m_body_holder.world, origin, direction, min_distance, max_distance);
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