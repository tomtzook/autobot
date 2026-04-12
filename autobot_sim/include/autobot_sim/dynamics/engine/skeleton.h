#pragma once

#include "autobot_sim/dynamics/engine/base.h"
#include "autobot_sim/dynamics/engine/world.h"

namespace autobot::sim::dynamics::engine {

struct ligament_holder {
    ligament_holder(const world_holder& world, const raw_body_type& body, raw_body_node_type node)
        : world(world)
        , body(body)
        , node(node)
    {}

    world_holder world;
    raw_body_type body;
    raw_body_node_type node;
};

template<joint_type joint_t_>
struct jointed_ligament_holder : public ligament_holder {
    jointed_ligament_holder(const world_holder& world, const raw_body_type& body, raw_body_node_type node, raw_joint_type<joint_t_> joint)
        : ligament_holder(world, body, node)
        , joint(joint)
    {}

    raw_joint_type<joint_t_> joint;
};

struct body_holder {
    world_holder world;
    raw_body_type body;
    jointed_ligament_holder<free_joint> root_ligament;
};

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

template<shape_type shape_t_, joint_type joint_t_>
std::pair<raw_joint_type<joint_t_>, raw_body_node_type> create_ligament(
    const world_holder& world,
    const raw_body_type& body,
    const std::optional<std::reference_wrapper<raw_body_node_type>> parent,
    const std::string_view name,
    const shape_t_& shape,
    const joint_t_& joint,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    using joint_info = joint_info<joint_t_>;
    using underlying_joint_type = joint_info::type;
    using shape_info = shape_info<shape_t_>;

    auto joint_props = joint_info::create_props(joint);
    joint_props.mName = name;
    joint_props.mT_ParentBodyToJoint = transform;

    dart::dynamics::BodyNode::Properties body_props;
    body_props.mName = name;

    const auto parent_raw = parent.has_value() ? parent.value().get() : nullptr;
    auto [created_joint, body_node] =
            body->createJointAndBodyNodePair<underlying_joint_type>(parent_raw, joint_props, body_props);

    if constexpr (!std::is_same_v<shape_t_, empty_shape>) {
        auto shape_ptr = shape_info::create(shape);
        auto shape_node = body_node->createShapeNode(shape_ptr);
        create_aspects(shape_node, aspects);
    }

    world.collision_group->addShapeFramesOf(body_node);

    return {created_joint, body_node};
}

template<shape_type shape_t_, joint_type joint_t_>
jointed_ligament_holder<joint_t_> create_ligament(
    ligament_holder& parent,
    const std::string_view name,
    const shape_t_& shape_info,
    const joint_t_& joint_info,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    auto [raw_joint, raw_body_node] = create_ligament(parent.world, parent.body, parent.node, name, shape_info, joint_info, transform, aspects);
    return jointed_ligament_holder<joint_t_>{parent.world, parent.body, raw_body_node, raw_joint};
}

template<shape_type shape_t_, joint_type joint_t_>
jointed_ligament_holder<joint_t_> create_root_ligament(
    const world_holder& world,
    const raw_body_type& body,
    const std::string_view name,
    const shape_t_& shape_info,
    const joint_t_& joint_info,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    auto [raw_joint, raw_body_node] = create_ligament(world, body, std::nullopt, name, shape_info, joint_info, transform, aspects);
    return jointed_ligament_holder<joint_t_>{world, body, raw_body_node, raw_joint};
}

body_holder create_body(const world_holder& world, std::string_view name);

}
