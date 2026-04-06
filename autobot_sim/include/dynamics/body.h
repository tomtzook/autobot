#pragma once

#include "body.h"
#include "dynamics/engine.h"

namespace autobot::sim::dynamics {

template<joint_type joint_t_>
class world_transformable {
public:
    explicit world_transformable(ligament_holder<joint_t_> ligament);

    [[nodiscard]] Eigen::Isometry3d get_world_transform() const;
    [[nodiscard]] Eigen::Vector3d get_world_linear_velocity() const;
    [[nodiscard]] Eigen::Vector3d get_world_angular_velocity() const;
    [[nodiscard]] Eigen::Vector3d get_world_linear_acceleration() const;
    [[nodiscard]] Eigen::Vector3d get_world_angular_acceleration() const;

protected:
    ligament_holder<joint_t_> m_ligament;
};

template<joint_type>
class transformable{};

template<>
class transformable<weld_joint> : public world_transformable<weld_joint> {
public:
    using joint_info = joint_info<weld_joint>;

    explicit transformable(ligament_holder<weld_joint> ligament);
};

template<>
class transformable<revolute_joint> : public world_transformable<revolute_joint> {
public:
    using joint_info = joint_info<revolute_joint>;

    explicit transformable(ligament_holder<revolute_joint> ligament);

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
    using joint_info = joint_info<prismatic_joint>;

    explicit transformable(ligament_holder<prismatic_joint> ligament);

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
    using joint_info = joint_info<ball_joint>;

    explicit transformable(ligament_holder<ball_joint> ligament);

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
    using joint_info = joint_info<free_joint>;

    explicit transformable(ligament_holder<free_joint> ligament);

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
    ligament(const body_holder& body_holder, ligament_holder<joint_t_> holder);

    ligament(const ligament&) = delete;
    ligament(ligament&&) = default;
    ligament& operator=(const ligament&) = delete;
    ligament& operator=(ligament&&) = default;

    template<shape_type shape_t_, joint_type joint_t2_>
    ligament<joint_t2_> attach(std::string_view name,
        const shape_t_& shape, const joint_t2_& joint,
        const Eigen::Isometry3d& transform,
        ligament_aspect aspects = ligament_aspect::none);

private:
    body_holder m_body_holder;
    ligament_holder<joint_t_> m_holder;

    friend class container;
};

class container : public transformable<free_joint> {
public:
    explicit container(std::string_view name);

    template<shape_type shape_t_, joint_type joint_t_>
    ligament<joint_t_> attach(std::string_view name,
        const shape_t_& shape, const joint_t_& joint,
        const Eigen::Isometry3d& transform,
        ligament_aspect aspects = ligament_aspect::none);

private:
    explicit container(body_holder&& body);
    container(const body_holder& body, ligament_holder<free_joint>&& root);

    static ligament_holder<free_joint> create_root(body_holder& body);

    body_holder m_holder;
    ligament<free_joint> m_root;

    friend class world;
};

class world {
public:
    world() = default;
    world(const world&) = delete;
    world(world&&) = default;
    world& operator=(const world&) = delete;
    world& operator=(world&&) = default;

    void add(const container& container);
    void step();

    void render(std::function<void(const Eigen::Matrix4d&, visual_shape)>&& render_action);

private:
    world_holder m_holder;
};

template<joint_type joint_t_>
world_transformable<joint_t_>::world_transformable(ligament_holder<joint_t_> ligament)
    : m_ligament(std::move(ligament))
{}

template<joint_type joint_t_>
Eigen::Isometry3d world_transformable<joint_t_>::get_world_transform() const {
    return m_ligament.body_node->getWorldTransform();
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
ligament<joint_t_>::ligament(const body_holder& body_holder, ligament_holder<joint_t_> holder)
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

template<shape_type shape_t_, joint_type joint_t_>
ligament<joint_t_> container::attach(
    const std::string_view name,
    const shape_t_& shape, const joint_t_& joint,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    return m_root.attach(name, shape, joint, transform, aspects);
}

}