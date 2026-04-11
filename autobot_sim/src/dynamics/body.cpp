
#include "dynamics/body.h"

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

transformable<weld_joint>::transformable(const engine::ligament_holder<weld_joint> ligament)
    : world_transformable<weld_joint>(ligament)
{}

transformable<revolute_joint>::transformable(const engine::ligament_holder<revolute_joint> ligament)
    : world_transformable<revolute_joint>(ligament)
{}

units::radians transformable<revolute_joint>::get_position() const {
    return units::radians{joint_info::get_pos(m_ligament.joint)};
}

void transformable<revolute_joint>::set_position(const units::radians value) {
    joint_info::set_pos(m_ligament.joint, value.value());
}

units::radians_per_second transformable<revolute_joint>::get_velocity() const {
    return units::radians_per_second{joint_info::get_vel(m_ligament.joint)};
}

void transformable<revolute_joint>::set_velocity(const units::radians_per_second value) {
    joint_info::set_vel(m_ligament.joint, value.value());
}

units::radians_per_second_squared transformable<revolute_joint>::get_acceleration() const {
    return units::radians_per_second_squared{joint_info::get_accel(m_ligament.joint)};
}

void transformable<revolute_joint>::set_acceleration(const units::radians_per_second_squared value) {
    joint_info::set_accel(m_ligament.joint, value.value());
}

units::meters transformable<prismatic_joint>::get_position() const {
    return units::meters{joint_info::get_pos(m_ligament.joint)};
}

void transformable<prismatic_joint>::set_position(const units::meters value) {
    joint_info::set_pos(m_ligament.joint, value.value());
}

transformable<prismatic_joint>::transformable(const engine::ligament_holder<prismatic_joint> ligament)
    : world_transformable<prismatic_joint>(ligament)
{}

units::meters_per_second transformable<prismatic_joint>::get_velocity() const {
    return units::meters_per_second{joint_info::get_vel(m_ligament.joint)};
}

void transformable<prismatic_joint>::set_velocity(const units::meters_per_second value) {
    joint_info::set_vel(m_ligament.joint, value.value());
}

units::meters_per_second_squared transformable<prismatic_joint>::get_acceleration() const {
    return units::meters_per_second_squared{joint_info::get_accel(m_ligament.joint)};
}

void transformable<prismatic_joint>::set_acceleration(const units::meters_per_second_squared value) {
    joint_info::set_accel(m_ligament.joint, value.value());
}

transformable<ball_joint>::transformable(const engine::ligament_holder<ball_joint> ligament)
    : world_transformable<ball_joint>(ligament)
{}

Eigen::Vector3<units::radians> transformable<ball_joint>::get_position() const {
    return wrap<units::radians, 3>(joint_info::get_pos(m_ligament.joint));
}

void transformable<ball_joint>::set_position(const Eigen::Vector3<units::radians>& value) {
    joint_info::set_pos(m_ligament.joint, unwrap<units::radians, 3>(value));
}

Eigen::Vector3<units::radians_per_second> transformable<ball_joint>::get_velocity() const {
    return wrap<units::radians_per_second, 3>(joint_info::get_vel(m_ligament.joint));
}

void transformable<ball_joint>::set_velocity(const Eigen::Vector3<units::radians_per_second>& value) {
    joint_info::set_vel(m_ligament.joint, unwrap<units::radians_per_second, 3>(value));
}

Eigen::Vector3<units::radians_per_second_squared> transformable<ball_joint>::get_acceleration() const {
    return wrap<units::radians_per_second_squared, 3>(joint_info::get_accel(m_ligament.joint));
}

void transformable<ball_joint>::set_acceleration(const Eigen::Vector3<units::radians_per_second_squared>& value) {
    joint_info::set_accel(m_ligament.joint, unwrap<units::radians_per_second_squared, 3>(value));
}

transformable<free_joint>::transformable(const engine::ligament_holder<free_joint> ligament)
    : world_transformable<autobot::sim::dynamics::free_joint>(ligament)
{}

Eigen::Vector3<units::meters> transformable<free_joint>::get_linear_position() const {
    return wrap<units::meters, 3>(joint_info::get_pos(m_ligament.joint).segment<3>(0));
}

void transformable<free_joint>::set_linear_position(const Eigen::Vector3<units::meters>& value) {
    auto data = joint_info::get_pos(m_ligament.joint);
    data.segment<3>(0) = unwrap<units::meters, 3>(value);
    joint_info::set_pos(m_ligament.joint, data);
}

Eigen::Vector3<units::meters_per_second> transformable<free_joint>::get_linear_velocity() const {
    return wrap<units::meters_per_second, 3>(joint_info::get_vel(m_ligament.joint).segment<3>(0));
}

void transformable<free_joint>::set_linear_velocity(const Eigen::Vector3<units::meters_per_second>& value) {
    auto data = joint_info::get_vel(m_ligament.joint);
    data.segment<3>(0) = unwrap<units::meters_per_second, 3>(value);
    joint_info::set_vel(m_ligament.joint, data);
}

Eigen::Vector3<units::meters_per_second_squared> transformable<free_joint>::get_linear_acceleration() const {
    return wrap<units::meters_per_second_squared, 3>(joint_info::get_vel(m_ligament.joint).segment<3>(0));
}

void transformable<free_joint>::set_linear_acceleration(const Eigen::Vector3<units::meters_per_second_squared>& value) {
    auto data = joint_info::get_accel(m_ligament.joint);
    data.segment<3>(0) = unwrap<units::meters_per_second_squared, 3>(value);
    joint_info::set_accel(m_ligament.joint, data);
}

Eigen::Vector3<units::radians> transformable<free_joint>::get_angular_position() const {
    return wrap<units::radians, 3>(joint_info::get_pos(m_ligament.joint).segment<3>(3));
}

void transformable<free_joint>::set_angular_position(const Eigen::Vector3<units::radians>& value) {
    auto data = joint_info::get_pos(m_ligament.joint);
    data.segment<3>(3) = unwrap<units::radians, 3>(value);
    joint_info::set_accel(m_ligament.joint, data);
}

Eigen::Vector3<units::radians_per_second> transformable<free_joint>::get_angular_velocity() const {
    return wrap<units::radians_per_second, 3>(joint_info::get_vel(m_ligament.joint).segment<3>(3));
}

void transformable<free_joint>::set_angular_velocity(const Eigen::Vector3<units::radians_per_second>& value) {
    auto data = joint_info::get_vel(m_ligament.joint);
    data.segment<3>(3) = unwrap<units::radians_per_second, 3>(value);
    joint_info::set_accel(m_ligament.joint, data);
}

Eigen::Vector3<units::radians_per_second_squared> transformable<free_joint>::get_angular_acceleration() const {
    return wrap<units::radians_per_second_squared, 3>(joint_info::get_accel(m_ligament.joint).segment<3>(3));
}

void transformable<free_joint>::set_angular_acceleration(const Eigen::Vector3<units::radians_per_second_squared>& value) {
    auto data = joint_info::get_accel(m_ligament.joint);
    data.segment<3>(3) = unwrap<units::radians_per_second_squared, 3>(value);
    joint_info::set_accel(m_ligament.joint, data);
}

container::container(engine::body_holder&& body)
    : container(body, create_root(body))
{}

container::container(const engine::body_holder& body, engine::ligament_holder<free_joint>&& root)
    : transformable<free_joint>(root)
    , m_holder(body)
    , m_root(ligament<free_joint>{m_holder, root})
{}

engine::ligament_holder<free_joint> container::create_root(const engine::body_holder& body) {
    return engine::create_ligament(body, nullptr, "root", empty_shape{}, free_joint{}, Eigen::Isometry3d::Identity(), ligament_aspect::all);
}

world::world()
    : m_holder()
{}

container world::create(const std::string_view name) {
    return container(engine::create_body(m_holder, name));
}

std::optional<raycast_result> world::raycast(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, const double max_distance) const {
    return engine::raycast(m_holder, origin, direction, max_distance);
}

void world::step() {
    world_step(m_holder);
}

void world::render(engine::render_function&& render_action) {
    world_render(m_holder, std::move(render_action));
}

}
