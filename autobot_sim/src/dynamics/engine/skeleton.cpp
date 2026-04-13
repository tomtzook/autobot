
#include "dynamics/engine/info.h"
#include "dynamics/engine/holders.h"


#include "autobot_sim/dynamics/engine/skeleton.h"

namespace autobot::sim::dynamics::engine {
dart_base_ligament_holder::dart_base_ligament_holder(const world_holder_ptr& world, const body_holder_ptr& body, dart::dynamics::BodyNode* node)
    : ligament_holder()
    , world(world)
    , body(body)
    , node(node)
{}

dart_body_holder::dart_body_holder(const world_holder_ptr& world, const dart::dynamics::SkeletonPtr& body)
    : body_holder()
    , world(world)
    , body(body)
{}

inline void create_aspects(dart::dynamics::ShapeNode* node, const ligament_aspect aspects) {
    if ((aspects & ligament_aspect::visual) != ligament_aspect::none) {
        node->createVisualAspect();
    }
    if ((aspects & ligament_aspect::collision) != ligament_aspect::none) {
        node->createCollisionAspect();
    }
    if ((aspects & ligament_aspect::dynamics) != ligament_aspect::none) {
        node->createCollisionAspect();
    }
}

template<shape_type shape_t_>
void create_shape_node(dart::dynamics::BodyNode* body_node, const shape_t_& shape, const ligament_aspect aspects) {
    using shape_info = dart_shape_info<shape_t_>;

    if constexpr (!std::is_same_v<shape_t_, empty_shape>) {
        auto shape_ptr = shape_info::create(shape);
        auto shape_node = body_node->createShapeNode(shape_ptr);
        create_aspects(shape_node, aspects);
    }
}

template<joint_type joint_t_>
std::pair<raw_joint_type<joint_t_>, dart::dynamics::BodyNode*> create_ligament_raw(
    const dart_world_holder& world,
    const dart::dynamics::SkeletonPtr& body,
    const std::optional<dart::dynamics::BodyNode*> parent,
    const std::string_view name,
    const generic_shape& shape,
    const joint_t_& joint,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    using joint_info = dart_joint_info<joint_t_>;
    using underlying_joint_type = joint_info::type;

    auto joint_props = joint_info::create_props(joint);
    joint_props.mName = name;
    joint_props.mT_ParentBodyToJoint = transform;

    dart::dynamics::BodyNode::Properties body_props;
    body_props.mName = name;

    const auto parent_raw = parent.has_value() ? parent.value() : nullptr;
    auto [created_joint, body_node] =
            body->createJointAndBodyNodePair<underlying_joint_type>(parent_raw, joint_props, body_props);

    std::visit([body_node, &aspects](const auto& val)->void {
        create_shape_node(body_node, val, aspects);
    }, shape);

    world.collision_group->addShapeFramesOf(body_node);

    return {created_joint, body_node};
}

template<joint_type joint_t_>
ligament_holder_ptr create_root_ligament(
    const world_holder_ptr& world,
    const body_holder_ptr& body,
    const std::string_view name,
    const generic_shape& shape_info,
    const joint_t_& joint_info,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    const auto& world_raw = holder(world);
    const auto& body_raw = holder(body);

    auto [raw_joint, raw_body_node] = create_ligament_raw(world_raw, body_raw, std::nullopt, name, shape_info, joint_info, transform, aspects);
    return std::make_shared<dart_ligament_holder<joint_t_>>(world, body, raw_body_node, raw_joint);
}

template<joint_type joint_t_>
ligament_holder_ptr create_ligament_generic(
    const ligament_holder_ptr& parent,
    const std::string_view name,
    const generic_shape& shape_info,
    const joint_t_& joint_info,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    const auto& parent_raw = holder(parent);
    const auto& world_raw = holder(parent_raw.world);
    const auto& body_raw = holder(parent_raw.body);

    auto [raw_joint, raw_body_node] = create_ligament_raw(world_raw, body_raw.body, parent_raw.node, name, shape_info, joint_info, transform, aspects);
    return std::make_shared<dart_ligament_holder<joint_t_>>(parent_raw.world, parent_raw.body, raw_body_node, raw_joint);
}

ligament_holder_ptr create_root_ligament(
    const body_holder_ptr& body,
    const std::string_view name,
    const ligament_aspect aspects) {
    const auto& body_raw = holder(body);
    const auto& world_raw = holder(body_raw.world);

    auto [raw_joint, raw_body_node] = create_ligament_raw(world_raw, body_raw.body, std::nullopt, name, empty_shape{}, free_joint{}, Eigen::Isometry3d::Identity(), aspects);
    return std::make_shared<dart_ligament_holder<free_joint>>(body_raw.world, body, raw_body_node, raw_joint);
}

ligament_holder_ptr create_ligament(
    const ligament_holder_ptr& parent,
    const std::string_view name,
    const generic_shape& shape_info,
    const weld_joint& joint_info,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    return create_ligament_generic(parent, name, shape_info, joint_info, transform, aspects);
}

ligament_holder_ptr create_ligament(
    const ligament_holder_ptr& parent,
    const std::string_view name,
    const generic_shape& shape_info,
    const revolute_joint& joint_info,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    return create_ligament_generic(parent, name, shape_info, joint_info, transform, aspects);
}

ligament_holder_ptr create_ligament(
    const ligament_holder_ptr& parent,
    const std::string_view name,
    const generic_shape& shape_info,
    const prismatic_joint& joint_info,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    return create_ligament_generic(parent, name, shape_info, joint_info, transform, aspects);
}

ligament_holder_ptr create_ligament(
    const ligament_holder_ptr& parent,
    const std::string_view name,
    const generic_shape& shape_info,
    const ball_joint& joint_info,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    return create_ligament_generic(parent, name, shape_info, joint_info, transform, aspects);
}

ligament_holder_ptr create_ligament(
    const ligament_holder_ptr& parent,
    const std::string_view name,
    const generic_shape& shape_info,
    const free_joint& joint_info,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    return create_ligament_generic(parent, name, shape_info, joint_info, transform, aspects);
}

body_holder_ptr create_body(const world_holder_ptr& world, const std::string_view name) {
    const auto& world_raw = holder(world);

    auto skeleton = dart::dynamics::Skeleton::create(name.data());
    auto body = std::make_shared<dart_body_holder>(world, skeleton);
    world_raw.world->addSkeleton(body->body);

    return body;
}

world_holder_ptr get_world(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder(ptr);
    return ligament.world;
}

Eigen::Isometry3d ligament_info::get_world_transform(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder(ptr);
    return ligament.node->getWorldTransform();
}

Eigen::Vector3d ligament_info::get_world_position(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder(ptr);
    return ligament.node->getWorldTransform().translation();
}

Eigen::Matrix3d ligament_info::get_world_rotation(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder(ptr);
    return ligament.node->getWorldTransform().linear();
}

Eigen::Vector3d ligament_info::get_world_linear_velocity(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder(ptr);
    return ligament.node->getLinearVelocity();
}

Eigen::Vector3d ligament_info::get_world_angular_velocity(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder(ptr);
    return ligament.node->getAngularVelocity();
}

Eigen::Vector3d ligament_info::get_world_linear_acceleration(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder(ptr);
    return ligament.node->getLinearAcceleration();
}

Eigen::Vector3d ligament_info::get_world_angular_acceleration(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder(ptr);
    return ligament.node->getAngularAcceleration();
}

template<joint_type joint_t_>
typename joint_info<joint_t_>::vector joint_get_position(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder_joint<joint_t_>(ptr);
    return ligament.joint->getPositionsStatic();
}

template<joint_type joint_t_>
void joint_set_position(const ligament_holder_ptr& ptr, const typename joint_info<joint_t_>::vector& value) {
    auto& ligament = holder_joint<joint_t_>(ptr);
    return ligament.joint->setPositionsStatic(value);
}

template<joint_type joint_t_>
typename joint_info<joint_t_>::vector joint_get_velocity(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder_joint<joint_t_>(ptr);
    return ligament.joint->getVelocitiesStatic();
}

template<joint_type joint_t_>
void joint_set_velocity(const ligament_holder_ptr& ptr, const typename joint_info<joint_t_>::vector& value) {
    auto& ligament = holder_joint<joint_t_>(ptr);
    return ligament.joint->setVelocitiesStatic(value);
}

template<joint_type joint_t_>
typename joint_info<joint_t_>::vector joint_get_acceleration(const ligament_holder_ptr& ptr) {
    const auto& ligament = holder_joint<joint_t_>(ptr);
    return ligament.joint->getAccelerationsStatic();
}

template<joint_type joint_t_>
void joint_set_acceleration(const ligament_holder_ptr& ptr, const typename joint_info<joint_t_>::vector& value) {
    auto& ligament = holder_joint<joint_t_>(ptr);
    return ligament.joint->setAccelerationsStatic(value);
}

template<joint_type joint_t_>
void joint_set_command(const ligament_holder_ptr& ptr, const typename joint_info<joint_t_>::vector& value) {
    auto& ligament = holder_joint<joint_t_>(ptr);
    return ligament.joint->setCommands(value);
}

#define joint_info_getters_settings(joint_type) \
    joint_info<joint_type>::vector joint_info<joint_type>::get_position(const ligament_holder_ptr& ptr) { return joint_get_position<joint_type>(ptr); } \
    void joint_info<joint_type>::set_position(const ligament_holder_ptr& ptr, const vector& value) { joint_set_position<joint_type>(ptr, value); } \
    joint_info<joint_type>::vector joint_info<joint_type>::get_velocity(const ligament_holder_ptr& ptr) { return joint_get_velocity<joint_type>(ptr); } \
    void joint_info<joint_type>::set_velocity(const ligament_holder_ptr& ptr, const vector& value) { joint_set_velocity<joint_type>(ptr, value); } \
    joint_info<joint_type>::vector joint_info<joint_type>::get_acceleration(const ligament_holder_ptr& ptr) { return joint_get_acceleration<joint_type>(ptr); } \
    void joint_info<joint_type>::set_acceleration(const ligament_holder_ptr& ptr, const vector& value) { joint_set_acceleration<joint_type>(ptr, value); } \
    void joint_info<joint_type>::set_command(const ligament_holder_ptr& ptr, const vector& value) { joint_set_command<joint_type>(ptr, value); }


joint_info_getters_settings(revolute_joint);
joint_info_getters_settings(prismatic_joint);
joint_info_getters_settings(ball_joint);
joint_info_getters_settings(free_joint);

}
