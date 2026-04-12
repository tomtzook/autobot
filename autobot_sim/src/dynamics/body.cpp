
#include "autobot_sim/dynamics/body.h"

namespace autobot::sim::dynamics {

joint<weld_joint>::joint(const raw_type& underlying)
    : base_joint<weld_joint>(underlying)
{}


joint<revolute_joint>::joint(const raw_type& underlying)
    : base_joint<revolute_joint>(underlying)
{}

units::radians joint<revolute_joint>::get_position() const {
    return units::radians(joint_info::get_position(m_underlying.joint));
}

void joint<revolute_joint>::set_position(const units::radians value) {
    joint_info::set_position(m_underlying.joint, value.value());
}

units::radians_per_second joint<revolute_joint>::get_velocity() const {
    return units::radians_per_second(joint_info::get_velocity(m_underlying.joint));
}

void joint<revolute_joint>::set_velocity(const units::radians_per_second value) {
    joint_info::set_velocity(m_underlying.joint, value.value());
}

units::radians_per_second_squared joint<revolute_joint>::get_acceleration() const {
    return units::radians_per_second_squared(joint_info::get_acceleration(m_underlying.joint));
}

void joint<revolute_joint>::set_acceleration(const units::radians_per_second_squared value) {
    joint_info::set_acceleration(m_underlying.joint, value.value());
}

joint<prismatic_joint>::joint(const raw_type& underlying)
    : base_joint<prismatic_joint>(underlying)
{}

units::meters joint<prismatic_joint>::get_position() const {
    return units::meters(joint_info::get_position(m_underlying.joint));
}

void joint<prismatic_joint>::set_position(const units::meters value) {
    joint_info::set_position(m_underlying.joint, value.value());
}

units::meters_per_second joint<prismatic_joint>::get_velocity() const {
    return units::meters_per_second(joint_info::get_velocity(m_underlying.joint));
}

void joint<prismatic_joint>::set_velocity(const units::meters_per_second value) {
    joint_info::set_velocity(m_underlying.joint, value.value());
}

units::meters_per_second_squared joint<prismatic_joint>::get_acceleration() const {
    return units::meters_per_second_squared(joint_info::get_acceleration(m_underlying.joint));
}

void joint<prismatic_joint>::set_acceleration(const units::meters_per_second_squared value) {
    joint_info::set_acceleration(m_underlying.joint, value.value());
}

joint<ball_joint>::joint(const raw_type& underlying)
    : base_joint<ball_joint>(underlying)
{}

math::rotation3 joint<ball_joint>::get_rotation() const {
    return math::rotation3{get_angular_position()};
}

void joint<ball_joint>::set_rotation(const math::rotation3& value) {
    set_angular_position(value.euler());
}

math::angular_position3<units::radians> joint<ball_joint>::get_angular_position() const {
    return math::angular_position3<units::radians>{joint_info::get_position(m_underlying.joint)};
}

void joint<ball_joint>::set_angular_position(const math::angular_position3<units::radians>& value) {
    joint_info::set_position(m_underlying.joint, value.raw());
}

math::angular_velocity3<units::radians_per_second> joint<ball_joint>::get_angular_velocity() const {
    return math::angular_velocity3<units::radians_per_second>{joint_info::get_velocity(m_underlying.joint)};
}

void joint<ball_joint>::set_angular_velocity(const math::angular_velocity3<units::radians_per_second>& value) {
    joint_info::set_velocity(m_underlying.joint, value.raw());
}

math::angular_acceleration3<units::radians_per_second_squared> joint<ball_joint>::get_angular_acceleration() const {
    return math::angular_acceleration3<units::radians_per_second_squared>{joint_info::get_acceleration(m_underlying.joint)};
}

void joint<ball_joint>::set_angular_acceleration(const math::angular_acceleration3<units::radians_per_second_squared>& value) {
    joint_info::set_acceleration(m_underlying.joint, value.raw());
}

joint<free_joint>::joint(const raw_type& underlying)
    : base_joint<free_joint>(underlying)
{}

math::translation3 joint<free_joint>::get_translation() const {
    return math::translation3{joint_info::get_linear_position(m_underlying.joint)};
}

void joint<free_joint>::set_translation(const math::translation3& value) {
    joint_info::set_linear_position(m_underlying.joint, value.raw());
}

math::rotation3 joint<free_joint>::get_rotation() const {
    return math::rotation3{get_angular_position()};
}

void joint<free_joint>::set_rotation(const math::rotation3& value) {
    set_angular_position(value.euler());
}

math::linear_position3<units::meters> joint<free_joint>::get_linear_position() const {
    return math::linear_position3<units::meters>{joint_info::get_linear_position(m_underlying.joint)};
}

void joint<free_joint>::set_linear_position(const math::linear_position3<units::meters>& value) {
    joint_info::set_linear_position(m_underlying.joint, value.raw());
}

math::linear_velocity3<units::meters_per_second> joint<free_joint>::get_linear_velocity() const {
    return math::linear_velocity3<units::meters_per_second>{joint_info::get_linear_velocity(m_underlying.joint)};
}

void joint<free_joint>::set_linear_velocity(const math::linear_velocity3<units::meters_per_second>& value) {
    joint_info::set_linear_velocity(m_underlying.joint, value.raw());
}

math::linear_acceleration3<units::meters_per_second_squared> joint<free_joint>::get_linear_acceleration() const {
    return math::linear_acceleration3<units::meters_per_second_squared>{joint_info::get_linear_acceleration(m_underlying.joint)};
}

void joint<free_joint>::set_linear_acceleration(const math::linear_acceleration3<units::meters_per_second_squared>& value) {
    joint_info::set_linear_acceleration(m_underlying.joint, value.raw());
}

math::angular_position3<units::radians> joint<free_joint>::get_angular_position() const {
    return math::angular_position3<units::radians>{joint_info::get_angular_position(m_underlying.joint)};
}

void joint<free_joint>::set_angular_position(const math::angular_position3<units::radians>& value) {
    joint_info::set_angular_position(m_underlying.joint, value.raw());
}

math::angular_velocity3<units::radians_per_second> joint<free_joint>::get_angular_velocity() const {
    return math::angular_velocity3<units::radians_per_second>{joint_info::get_angular_velocity(m_underlying.joint)};
}

void joint<free_joint>::set_angular_velocity(const math::angular_velocity3<units::radians_per_second>& value) {
    joint_info::set_angular_velocity(m_underlying.joint, value.raw());
}

math::angular_acceleration3<units::radians_per_second_squared> joint<free_joint>::get_angular_acceleration() const {
    return math::angular_acceleration3<units::radians_per_second_squared>{joint_info::get_angular_acceleration(m_underlying.joint)};
}
void joint<free_joint>::set_angular_acceleration(const math::angular_acceleration3<units::radians_per_second_squared>& value) {
    joint_info::set_angular_acceleration(m_underlying.joint, value.raw());
}

math::translation3 ligament::get_translation() const {
    return math::translation3{engine::body_node::get_world_position(get_node().node)};
}

math::rotation3 ligament::get_rotation() const {
    return math::rotation3{engine::body_node::get_world_rotation(get_node().node)};
}

math::linear_velocity3<units::meters_per_second> ligament::get_linear_velocity() const {
    return math::linear_velocity3<units::meters_per_second>{engine::body_node::get_world_linear_velocity(get_node().node)};
}

math::linear_acceleration3<units::meters_per_second_squared> ligament::get_linear_acceleration() const {
    return math::linear_acceleration3<units::meters_per_second_squared>{engine::body_node::get_world_linear_acceleration(get_node().node)};
}

math::angular_velocity3<units::radians_per_second> ligament::get_angular_velocity() const {
    return math::angular_velocity3<units::radians_per_second>{engine::body_node::get_world_angular_velocity(get_node().node)};
}

math::angular_acceleration3<units::radians_per_second_squared> ligament::get_angular_acceleration() const {
    return math::angular_acceleration3<units::radians_per_second_squared>{engine::body_node::get_world_angular_acceleration(get_node().node)};
}

math::axis3 ligament::forward() const {
    const auto axis = math::forward();
    return get_rotation() * axis;
}

math::axis3 ligament::backward() const {
    const auto axis = math::backward();
    return get_rotation() * axis;
}

math::axis3 ligament::left() const {
    const auto axis = math::left();
    return get_rotation() * axis;
}

math::axis3 ligament::right() const {
    const auto axis = math::right();
    return get_rotation() * axis;
}

math::axis3 ligament::up() const {
    const auto axis = math::up();
    return get_rotation() * axis;
}

math::axis3 ligament::down() const {
    const auto axis = math::down();
    return get_rotation() * axis;
}

std::optional<units::meters> ligament::raycast(const math::translation3& origin_offset, const math::axis3& axis, const units::meters min, const units::meters max) const {
    const auto& node = get_node();
    const auto pos = get_translation();
    const auto origin = pos + origin_offset;
    if (const auto distance_opt = engine::raycast(node.world, origin.raw(), axis.raw(), min.value(), max.value()); distance_opt) {
        return units::meters(distance_opt.value());
    }

    return std::nullopt;
}

void ligament::attach(const engine::ligament_holder& node) {
    m_node = node;
}

const engine::ligament_holder& ligament::get_node() const {
    if (!m_node) {
        throw std::runtime_error("node not attached to body");
    }

    return m_node.value();
}

engine::ligament_holder& ligament::get_node() {
    if (!m_node) {
        throw std::runtime_error("node not attached to body");
    }

    return m_node.value();
}

const joint<free_joint>& body::get_joint() const {
    return m_joint;
}

joint<free_joint>& body::get_joint() {
    return m_joint;
}

body::body(engine::body_holder&& underlying)
    : ligament()
    , m_underlying(std::move(underlying))
    , m_joint(m_underlying.root_ligament) {
    attach(m_underlying.root_ligament);
}

body world::create_body(const std::string_view name) {
    return body{engine::create_body(m_underlying, name)};
}

void world::step() {
    engine::step_world(m_underlying);
}

void world::render(engine::render_function&& render_action) const {
    engine::render_world(m_underlying, std::move(render_action));
}

}
