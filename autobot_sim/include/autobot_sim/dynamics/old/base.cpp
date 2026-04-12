
#include <utility>

#include "autobot_sim/dynamics/base.h"

namespace autobot::sim::dynamics::engine {

static ligament<free_joint> create_root(const world_holder& world, const body_holder& body) {
    auto [joint_holder, body_node_holder] = create_ligament(world, body, std::nullopt, "root", empty_shape{}, free_joint{}, Eigen::Isometry3d::Identity(), ligament_aspect::all);
    return {world, body, std::move(joint_holder), std::move(body_node_holder)};
}

body_node::body_node(const world_holder& world, body_node_holder holder)
    : world(world)
    , holder(holder)
{}

Eigen::Isometry3d body_node::get_world_transform() const {
    return holder.body_node->getWorldTransform();
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
    return holder.body_node->getLinearVelocity();
}

Eigen::Vector3d body_node::get_world_angular_velocity() const {
    return holder.body_node->getAngularVelocity();
}

Eigen::Vector3d body_node::get_world_linear_acceleration() const {
    return holder.body_node->getLinearAcceleration();
}

Eigen::Vector3d body_node::get_world_angular_acceleration() const {
    return holder.body_node->getAngularAcceleration();
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

joint<weld_joint>::joint(const joint_holder<weld_joint> holder)
    : holder(holder)
{}

joint<revolute_joint>::joint(const joint_holder<revolute_joint> holder)
    : holder(holder)
{}

double joint<revolute_joint>::get_position() const {
    return holder.joint->getPosition(0);
}

void joint<revolute_joint>::set_position(const double value) {
    holder.joint->setPosition(0, value);
}

double joint<revolute_joint>::get_velocity() const {
    return holder.joint->getVelocity(0);
}

void joint<revolute_joint>::set_velocity(const double value) {
    holder.joint->setVelocity(0, value);
}

double joint<revolute_joint>::get_acceleration() const {
    return holder.joint->getAcceleration(0);
}

void joint<revolute_joint>::set_acceleration(const double value) {
    holder.joint->setAcceleration(0, value);
}

double joint<prismatic_joint>::get_position() const {
    return holder.joint->getPosition(0);
}

void joint<prismatic_joint>::set_position(const double value) {
    holder.joint->setPosition(0, value);
}

joint<prismatic_joint>::joint(const joint_holder<prismatic_joint> holder)
    : holder(holder)
{}

double joint<prismatic_joint>::get_position() const {
    return holder.joint->getPosition(0);
}

void joint<prismatic_joint>::set_position(const double value) {
    holder.joint->setPosition(0, value);
}

double joint<prismatic_joint>::get_velocity() const {
    return holder.joint->getVelocity(0);
}

void joint<prismatic_joint>::set_velocity(const double value) {
    holder.joint->setVelocity(0, value);
}

double joint<prismatic_joint>::get_acceleration() const {
    return holder.joint->getAcceleration(0);
}

void joint<prismatic_joint>::set_acceleration(const double value) {
    holder.joint->setAcceleration(0, value);
}

joint<ball_joint>::joint(const joint_holder<ball_joint> holder)
    : holder(holder)
{}

Eigen::Vector3d joint<ball_joint>::get_position() const {
    return holder.joint->getPositions();
}

void joint<ball_joint>::set_position(const Eigen::Vector3d& value) {
    holder.joint->setPositions(value);
}

Eigen::Vector3d joint<ball_joint>::get_velocity() const {
    return holder.joint->getVelocities();
}

void joint<ball_joint>::set_velocity(const Eigen::Vector3d& value) {
    holder.joint->setVelocities(value);
}

Eigen::Vector3d joint<ball_joint>::get_acceleration() const {
    return holder.joint->getAccelerations();
}

void joint<ball_joint>::set_acceleration(const Eigen::Vector3d& value) {
    holder.joint->setAccelerations(value);
}

joint<free_joint>::joint(const joint_holder<free_joint> holder)
    : holder(holder)
{}

Eigen::Vector3d joint<free_joint>::get_linear_position() const {
    return holder.joint->getPositions().segment<3>(0);
}

void joint<free_joint>::set_linear_position(const Eigen::Vector3d& value) {
    auto data = holder.joint->getPositions();
    data.segment<3>(0) = value;
    holder.joint->setPositions(data);
}

Eigen::Vector3d joint<free_joint>::get_linear_velocity() const {
    return holder.joint->getVelocities().segment<3>(0);
}

void joint<free_joint>::set_linear_velocity(const Eigen::Vector3d& value) {
    auto data = holder.joint->getVelocities();
    data.segment<3>(0) = value;
    holder.joint->setVelocities(data);
}

Eigen::Vector3d joint<free_joint>::get_linear_acceleration() const {
    return holder.joint->getAccelerations().segment<3>(0);
}

void joint<free_joint>::set_linear_acceleration(const Eigen::Vector3d& value) {
    auto data = holder.joint->getAccelerations();
    data.segment<3>(0) = value;
    holder.joint->setAccelerations(data);
}

Eigen::Vector3d joint<free_joint>::get_angular_position() const {
    return holder.joint->getPositions().segment<3>(3);
}

void joint<free_joint>::set_angular_position(const Eigen::Vector3d& value) {
    auto data = holder.joint->getPositions();
    data.segment<3>(3) = value;
    holder.joint->setPositions(data);
}

Eigen::Vector3d joint<free_joint>::get_angular_velocity() const {
    return holder.joint->getVelocities().segment<3>(3);
}

void joint<free_joint>::set_angular_velocity(const Eigen::Vector3d& value) {
    auto data = holder.joint->getVelocities();
    data.segment<3>(3) = value;
    holder.joint->setVelocities(data);
}

Eigen::Vector3d joint<free_joint>::get_angular_acceleration() const {
    return holder.joint->getAccelerations().segment<3>(3);
}

void joint<free_joint>::set_angular_acceleration(const Eigen::Vector3d& value) {
    auto data = holder.joint->getAccelerations();
    data.segment<3>(3) = value;
    holder.joint->setAccelerations(data);
}

body::body(const world_holder& world, body_holder&& body)
    : body(world, std::move(body), create_root(world, body))
{}

body::body(const world_holder& world, body_holder&& body, ligament<free_joint>&& root)
    : world(world)
    , holder(std::move(body))
    , root(std::move(root))
{}

world::world()
    : holder()
{}

body world::create(const std::string_view name) {
    return {holder, create_body(holder, name)};
}

void world::step() {
    world_step(holder);
}

void world::render(render_function&& render_action) {
    world_render(holder, std::move(render_action));
}

std::optional<raycast_result> raycast(
    const world_holder& world,
    const Eigen::Vector3d& origin,
    const Eigen::Vector3d& direction,
    const units::meters max_distance) {
    return raycast(world, origin, direction, units::meters(0), max_distance);
}

std::optional<raycast_result> raycast(
    const world_holder& world,
    const Eigen::Vector3d& origin,
    const Eigen::Vector3d& direction,
    const units::meters min_distance,
    const units::meters max_distance) {
    const auto to = origin + (direction * max_distance.value()).eval();

    dart::collision::RaycastOption option;
    option.mEnableAllHits = false;
    option.mSortByClosest = true;
    dart::collision::RaycastResult result;
    if (world.collision_group->raycast(origin, to, option, &result)) {
        const auto& first_hit = result.mRayHits[0];
        const auto hit_point = first_hit.mPoint;
        const auto hit_distance = units::meters((origin - hit_point).norm());
        if (hit_distance >= min_distance) {
            return raycast_result{hit_distance, hit_point};
        }
    }

    return std::nullopt;
}

}
