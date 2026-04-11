
#include "autobot_sim/dynamics/body.h"

namespace autobot::sim::dynamics {

template<typename dst_t_, size_t size_>
Eigen::Vector<dst_t_, size_> wrap(const auto& data) {
    Eigen::Vector<dst_t_, size_> result;
    std::transform(data.begin(), data.end(), result.begin(), [](auto d) { return dst_t_(d); });
    return result;
}

template<typename src_t_, size_t size_, typename dst_t_ = double>
Eigen::Vector<dst_t_, size_> unwrap(const Eigen::Vector<src_t_, size_>& data) {
    Eigen::Vector<dst_t_, size_> result;
    std::transform(data.begin(), data.end(), result.begin(), [](auto& d) { return static_cast<dst_t_>(d); });
    return result;
}

body_node::body_node(engine::body_holder body_holder, engine::body_node_holder holder)
    : m_body_holder(std::move(body_holder))
    , m_holder(std::move(holder))
{}

Eigen::Isometry3d body_node::get_world_transform() const {
    return m_holder.body_node->getWorldTransform();
}

Eigen::Vector3d body_node::get_world_position() const {
    return get_world_transform().translation();
}

Eigen::Quaterniond body_node::get_world_rotation() const {
    const auto transform = get_world_transform();
    auto quat = Eigen::Quaterniond(transform.linear());
    quat.normalize();
    return quat;
}

Eigen::Vector3d body_node::get_world_linear_velocity() const {
    return m_holder.body_node->getLinearVelocity();
}

Eigen::Vector3d body_node::get_world_angular_velocity() const {
    return m_holder.body_node->getAngularVelocity();
}

Eigen::Vector3d body_node::get_world_linear_acceleration() const {
    return m_holder.body_node->getLinearAcceleration();
}

Eigen::Vector3d body_node::get_world_angular_acceleration() const {
    return m_holder.body_node->getAngularAcceleration();
}

Eigen::Vector3d body_node::forward() const {
    const auto axis = Eigen::Vector3d::UnitX();
    return get_world_rotation() * axis;
}

Eigen::Vector3d body_node::backward() const {
    const auto axis = -Eigen::Vector3d::UnitX();
    return get_world_rotation() * axis;
}

Eigen::Vector3d body_node::left() const {
    const auto axis = Eigen::Vector3d::UnitY();
    return get_world_rotation() * axis;
}

Eigen::Vector3d body_node::right() const {
    const auto axis = -Eigen::Vector3d::UnitY();
    return get_world_rotation() * axis;
}

Eigen::Vector3d body_node::up() const {
    const auto axis = Eigen::Vector3d::UnitZ();
    return get_world_rotation() * axis;
}

Eigen::Vector3d body_node::down() const {
    const auto axis = -Eigen::Vector3d::UnitZ();
    return get_world_rotation() * axis;
}

std::optional<raycast_result> body_node::raycast(const Eigen::Vector3d& origin_offset, const Eigen::Vector3d& direction, const units::meters max_distance) const {
    return raycast(origin_offset, direction, units::meters(0), max_distance);
}

std::optional<raycast_result> body_node::raycast(const Eigen::Vector3d& origin_offset, const Eigen::Vector3d& direction, const units::meters min_distance, const units::meters max_distance) const {
    const auto origin = get_world_position() + origin_offset;
    return engine::raycast(m_body_holder.world, origin, direction, min_distance, max_distance);
}

joint<weld_joint>::joint(const engine::joint_holder<weld_joint> joint)
    : m_joint(std::move(joint))
{}

joint<revolute_joint>::joint(const engine::joint_holder<revolute_joint> joint)
    : m_joint(std::move(joint))
{}

units::radians joint<revolute_joint>::get_position() const {
    return units::radians{joint_info::get_pos(m_joint.joint)};
}

void joint<revolute_joint>::set_position(const units::radians value) {
    joint_info::set_pos(m_joint.joint, value.value());
}

units::radians_per_second joint<revolute_joint>::get_velocity() const {
    return units::radians_per_second{joint_info::get_vel(m_joint.joint)};
}

void joint<revolute_joint>::set_velocity(const units::radians_per_second value) {
    joint_info::set_vel(m_joint.joint, value.value());
}

units::radians_per_second_squared joint<revolute_joint>::get_acceleration() const {
    return units::radians_per_second_squared{joint_info::get_accel(m_joint.joint)};
}

void joint<revolute_joint>::set_acceleration(const units::radians_per_second_squared value) {
    joint_info::set_accel(m_joint.joint, value.value());
}

units::meters joint<prismatic_joint>::get_position() const {
    return units::meters{joint_info::get_pos(m_joint.joint)};
}

void joint<prismatic_joint>::set_position(const units::meters value) {
    joint_info::set_pos(m_joint.joint, value.value());
}

joint<prismatic_joint>::joint(const engine::joint_holder<prismatic_joint> joint)
    : m_joint(std::move(joint))
{}

units::meters_per_second joint<prismatic_joint>::get_velocity() const {
    return units::meters_per_second{joint_info::get_vel(m_joint.joint)};
}

void joint<prismatic_joint>::set_velocity(const units::meters_per_second value) {
    joint_info::set_vel(m_joint.joint, value.value());
}

units::meters_per_second_squared joint<prismatic_joint>::get_acceleration() const {
    return units::meters_per_second_squared{joint_info::get_accel(m_joint.joint)};
}

void joint<prismatic_joint>::set_acceleration(const units::meters_per_second_squared value) {
    joint_info::set_accel(m_joint.joint, value.value());
}

joint<ball_joint>::joint(const engine::joint_holder<ball_joint> joint)
    : m_joint(std::move(joint))
{}

Eigen::Vector3<units::radians> joint<ball_joint>::get_position() const {
    return wrap<units::radians, 3>(joint_info::get_pos(m_joint.joint));
}

void joint<ball_joint>::set_position(const Eigen::Vector3<units::radians>& value) {
    joint_info::set_pos(m_joint.joint, unwrap<units::radians, 3>(value));
}

Eigen::Vector3<units::radians_per_second> joint<ball_joint>::get_velocity() const {
    return wrap<units::radians_per_second, 3>(joint_info::get_vel(m_joint.joint));
}

void joint<ball_joint>::set_velocity(const Eigen::Vector3<units::radians_per_second>& value) {
    joint_info::set_vel(m_joint.joint, unwrap<units::radians_per_second, 3>(value));
}

Eigen::Vector3<units::radians_per_second_squared> joint<ball_joint>::get_acceleration() const {
    return wrap<units::radians_per_second_squared, 3>(joint_info::get_accel(m_joint.joint));
}

void joint<ball_joint>::set_acceleration(const Eigen::Vector3<units::radians_per_second_squared>& value) {
    joint_info::set_accel(m_joint.joint, unwrap<units::radians_per_second_squared, 3>(value));
}

joint<free_joint>::joint(const engine::joint_holder<free_joint> joint)
    : m_joint(std::move(joint))
{}

Eigen::Vector3<units::meters> joint<free_joint>::get_linear_position() const {
    return wrap<units::meters, 3>(joint_info::get_pos(m_joint.joint).segment<3>(0));
}

void joint<free_joint>::set_linear_position(const Eigen::Vector3<units::meters>& value) {
    auto data = joint_info::get_pos(m_joint.joint);
    data.segment<3>(0) = unwrap<units::meters, 3>(value);
    joint_info::set_pos(m_joint.joint, data);
}

Eigen::Vector3<units::meters_per_second> joint<free_joint>::get_linear_velocity() const {
    return wrap<units::meters_per_second, 3>(joint_info::get_vel(m_joint.joint).segment<3>(0));
}

void joint<free_joint>::set_linear_velocity(const Eigen::Vector3<units::meters_per_second>& value) {
    auto data = joint_info::get_vel(m_joint.joint);
    data.segment<3>(0) = unwrap<units::meters_per_second, 3>(value);
    joint_info::set_vel(m_joint.joint, data);
}

Eigen::Vector3<units::meters_per_second_squared> joint<free_joint>::get_linear_acceleration() const {
    return wrap<units::meters_per_second_squared, 3>(joint_info::get_vel(m_joint.joint).segment<3>(0));
}

void joint<free_joint>::set_linear_acceleration(const Eigen::Vector3<units::meters_per_second_squared>& value) {
    auto data = joint_info::get_accel(m_joint.joint);
    data.segment<3>(0) = unwrap<units::meters_per_second_squared, 3>(value);
    joint_info::set_accel(m_joint.joint, data);
}

Eigen::Vector3<units::radians> joint<free_joint>::get_angular_position() const {
    return wrap<units::radians, 3>(joint_info::get_pos(m_joint.joint).segment<3>(3));
}

void joint<free_joint>::set_angular_position(const Eigen::Vector3<units::radians>& value) {
    auto data = joint_info::get_pos(m_joint.joint);
    data.segment<3>(3) = unwrap<units::radians, 3>(value);
    joint_info::set_accel(m_joint.joint, data);
}

Eigen::Vector3<units::radians_per_second> joint<free_joint>::get_angular_velocity() const {
    return wrap<units::radians_per_second, 3>(joint_info::get_vel(m_joint.joint).segment<3>(3));
}

void joint<free_joint>::set_angular_velocity(const Eigen::Vector3<units::radians_per_second>& value) {
    auto data = joint_info::get_vel(m_joint.joint);
    data.segment<3>(3) = unwrap<units::radians_per_second, 3>(value);
    joint_info::set_accel(m_joint.joint, data);
}

Eigen::Vector3<units::radians_per_second_squared> joint<free_joint>::get_angular_acceleration() const {
    return wrap<units::radians_per_second_squared, 3>(joint_info::get_accel(m_joint.joint).segment<3>(3));
}

void joint<free_joint>::set_angular_acceleration(const Eigen::Vector3<units::radians_per_second_squared>& value) {
    auto data = joint_info::get_accel(m_joint.joint);
    data.segment<3>(3) = unwrap<units::radians_per_second_squared, 3>(value);
    joint_info::set_accel(m_joint.joint, data);
}

container::container(engine::body_holder&& body)
    : container(body, create_root(body))
{}

container::container(const engine::body_holder& body, ligament<free_joint>&& root)
    : body_node(root.m_body_node)
    , m_holder(body)
    , m_root(std::move(root))
{}

ligament<free_joint> container::create_root(const engine::body_holder& body) {
    auto [joint_holder, body_node_holder] = engine::create_ligament(body, std::nullopt, "root", empty_shape{}, free_joint{}, Eigen::Isometry3d::Identity(), ligament_aspect::all);
    return {body, std::move(joint_holder), std::move(body_node_holder)};
}

world::world()
    : m_holder()
{}

container world::create(const std::string_view name) {
    return container(engine::create_body(m_holder, name));
}

std::optional<raycast_result> world::raycast(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, const units::meters max_distance) const {
    return engine::raycast(m_holder, origin, direction, units::meters(0), max_distance);
}

std::optional<raycast_result> world::raycast(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, const units::meters min_distance, const units::meters max_distance) const {
    return engine::raycast(m_holder, origin, direction, min_distance, max_distance);
}

void world::step() {
    world_step(m_holder);
}

void world::render(engine::render_function&& render_action) {
    world_render(m_holder, std::move(render_action));
}

}
