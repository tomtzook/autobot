#pragma once

#include <autobot/math/transform.h>

#include "autobot_sim/dynamics/engine/skeleton.h"
#include "autobot_sim/dynamics/engine/world.h"
#include "autobot_sim/dynamics/engine/raycast.h"

namespace autobot::sim::dynamics {

template<joint_type joint_t_>
class base_joint {
public:
    using joint_info = engine::joint_info<joint_t_>;
    using raw_type = engine::jointed_ligament_holder<joint_t_>;

    explicit base_joint(const raw_type& underlying) : m_underlying(underlying) {}

protected:
    raw_type m_underlying;
};

template<joint_type>
class joint{};

template<>
class joint<weld_joint> : public base_joint<weld_joint> {
public:
    explicit joint(const raw_type& underlying);
};

template<>
class joint<revolute_joint> : public base_joint<revolute_joint> {
public:
    explicit joint(const raw_type& underlying);

    [[nodiscard]] units::radians get_position() const;
    void set_position(units::radians value);
    [[nodiscard]] units::radians_per_second get_velocity() const;
    void set_velocity(units::radians_per_second value);
    [[nodiscard]] units::radians_per_second_squared get_acceleration() const;
    void set_acceleration(units::radians_per_second_squared value);
};

template<>
class joint<prismatic_joint> : public base_joint<prismatic_joint> {
public:
    explicit joint(const raw_type& underlying);

    [[nodiscard]] units::meters get_position() const;
    void set_position(units::meters value);
    [[nodiscard]] units::meters_per_second get_velocity() const;
    void set_velocity(units::meters_per_second value);
    [[nodiscard]] units::meters_per_second_squared get_acceleration() const;
    void set_acceleration(units::meters_per_second_squared value);
};

template<>
class joint<ball_joint> : public base_joint<ball_joint> {
public:
    explicit joint(const raw_type& underlying);

    [[nodiscard]] math::rotation3 get_rotation() const;
    void set_rotation(const math::rotation3& value);

    [[nodiscard]] math::angular_position3<units::radians> get_angular_position() const;
    void set_angular_position(const math::angular_position3<units::radians>& value);
    [[nodiscard]] math::angular_velocity3<units::radians_per_second> get_angular_velocity() const;
    void set_angular_velocity(const math::angular_velocity3<units::radians_per_second>& value);
    [[nodiscard]] math::angular_acceleration3<units::radians_per_second_squared> get_angular_acceleration() const;
    void set_angular_acceleration(const math::angular_acceleration3<units::radians_per_second_squared>& value);
};

template<>
class joint<free_joint> : public base_joint<free_joint> {
public:
    explicit joint(const raw_type& underlying);

    [[nodiscard]] math::translation3 get_translation() const;
    void set_translation(const math::translation3& value);
    [[nodiscard]] math::rotation3 get_rotation() const;
    void set_rotation(const math::rotation3& value);

    [[nodiscard]] math::linear_position3<units::meters> get_linear_position() const;
    void set_linear_position(const math::linear_position3<units::meters>& value);
    [[nodiscard]] math::linear_velocity3<units::meters_per_second> get_linear_velocity() const;
    void set_linear_velocity(const math::linear_velocity3<units::meters_per_second>& value);
    [[nodiscard]] math::linear_acceleration3<units::meters_per_second_squared> get_linear_acceleration() const;
    void set_linear_acceleration(const math::linear_acceleration3<units::meters_per_second_squared>& value);

    [[nodiscard]] math::angular_position3<units::radians> get_angular_position() const;
    void set_angular_position(const math::angular_position3<units::radians>& value);
    [[nodiscard]] math::angular_velocity3<units::radians_per_second> get_angular_velocity() const;
    void set_angular_velocity(const math::angular_velocity3<units::radians_per_second>& value);
    [[nodiscard]] math::angular_acceleration3<units::radians_per_second_squared> get_angular_acceleration() const;
    void set_angular_acceleration(const math::angular_acceleration3<units::radians_per_second_squared>& value);
};

class ligament {
public:
    ligament() = default;

    [[nodiscard]] math::translation3 get_translation() const;
    [[nodiscard]] math::rotation3 get_rotation() const;

    [[nodiscard]] math::linear_velocity3<units::meters_per_second> get_linear_velocity() const;
    [[nodiscard]] math::linear_acceleration3<units::meters_per_second_squared> get_linear_acceleration() const;
    [[nodiscard]] math::angular_velocity3<units::radians_per_second> get_angular_velocity() const;
    [[nodiscard]] math::angular_acceleration3<units::radians_per_second_squared> get_angular_acceleration() const;

    [[nodiscard]] math::axis3 forward() const;
    [[nodiscard]] math::axis3 backward() const;
    [[nodiscard]] math::axis3 left() const;
    [[nodiscard]] math::axis3 right() const;
    [[nodiscard]] math::axis3 up() const;
    [[nodiscard]] math::axis3 down() const;

    [[nodiscard]] std::optional<units::meters> raycast(const math::translation3& origin_offset, const math::axis3& axis, units::meters min, units::meters max) const;

    template<std::derived_from<ligament> t_, joint_type joint_t_>
    joint<joint_t_> attach(std::string_view name, t_& node, const joint_t_& joint_info, const math::transform3& transform = math::transform3());
    template<shape_type t_, joint_type joint_t_>
    std::pair<joint<joint_t_>, ligament> attach(std::string_view name, const t_& shape, const joint_t_& joint_info, const math::transform3& transform = math::transform3());

protected:
    void attach(const engine::ligament_holder& node);

private:
    [[nodiscard]] const engine::ligament_holder& get_node() const;
    [[nodiscard]] engine::ligament_holder& get_node();

    std::optional<engine::ligament_holder> m_node{};
};

class body : public ligament {
public:
    const joint<free_joint>& get_joint() const;
    joint<free_joint>& get_joint();

private:
    explicit body(engine::body_holder&& underlying);

    engine::body_holder m_underlying;
    joint<free_joint> m_joint;

    friend class world;
};

class world {
public:
    body create_body(std::string_view name);
    void step();
    void render(engine::render_function&& render_action) const;

private:
    engine::world_holder m_underlying;
};

template<std::derived_from<ligament> t_, joint_type joint_t_>
joint<joint_t_> ligament::attach(const std::string_view name, t_& node, const joint_t_& joint_info, const math::transform3& transform) {
    auto& this_node = get_node();
    auto ligament_holder = engine::create_ligament(
        this_node,
        name,
        t_::shape,
        joint_info,
        transform.raw(),
        ligament_aspect::all);
    node.attach(ligament_holder);

    return joint<joint_t_>{ligament_holder};
}

template<shape_type t_, joint_type joint_t_>
std::pair<joint<joint_t_>, ligament> ligament::attach(const std::string_view name, const t_& shape, const joint_t_& joint_info, const math::transform3& transform) {
    auto& this_node = get_node();
    auto ligament_holder = engine::create_ligament(
        this_node,
        name,
        shape,
        joint_info,
        transform.raw(),
        ligament_aspect::all);

    ligament ligament;
    ligament.attach(ligament_holder);

    return {joint<joint_t_>{ligament_holder}, std::move(ligament)};
}

}

