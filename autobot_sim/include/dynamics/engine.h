#pragma once

#include <dart/dart.hpp>
#include <dart/collision/bullet/BulletCollisionDetector.hpp>

#include "dynamics/data.h"

namespace autobot::sim::dynamics::engine {

enum class visual_shape {
    unknown,
    box,
    sphere,
};

template<shape_type>
struct shape_info {};

template<>
struct shape_info<box_shape> {
    using type = dart::dynamics::BoxShape;
    using type_ptr = std::shared_ptr<type>;

    static type_ptr create(const box_shape& shape) {
        return std::make_shared<type>(shape.dims());
    }
};

template<>
struct shape_info<sphere_shape> {
    using type = dart::dynamics::SphereShape;
    using type_ptr = std::shared_ptr<type>;

    static type_ptr create(const sphere_shape& shape) {
        return std::make_shared<type>(shape.radius.value());
    }
};

template<>
struct shape_info<cylinder_shape> {
    using type = dart::dynamics::CylinderShape;
    using type_ptr = std::shared_ptr<type>;

    static type_ptr create(const cylinder_shape& shape) {
        return std::make_shared<type>(shape.radius.value(), shape.height.value());
    }
};

template<>
struct shape_info<ellipsoid_shape> {
    using type = dart::dynamics::EllipsoidShape;
    using type_ptr = std::shared_ptr<type>;

    static type_ptr create(const ellipsoid_shape& shape) {
        return std::make_shared<type>(shape.dims());
    }
};

template<joint_type>
struct joint_info {};

template<>
struct joint_info<weld_joint> {
    using type = dart::dynamics::WeldJoint;
    using props = type::Properties;

    static props create_props(const weld_joint& joint) {
        props props;
        return props;
    }
};

template<>
struct joint_info<revolute_joint> {
    using type = dart::dynamics::RevoluteJoint;
    using props = type::Properties;

    static props create_props(const revolute_joint& joint) {
        props props;
        props.mAxis = joint.rotation_axis;
        return props;
    }

    static double get_pos(const type* joint) {
        return joint->getPosition(0);
    }
    static void set_pos(type* joint, const double value) {
        joint->setPosition(0, value);
    }
    static double get_vel(const type* joint) {
        return joint->getVelocity(0);
    }
    static void set_vel(type* joint, const double value) {
        joint->setVelocity(0, value);
    }
    static double get_accel(const type* joint) {
        return joint->getAcceleration(0);
    }
    static void set_accel(type* joint, const double value) {
        joint->setAcceleration(0, value);
    }
};

template<>
struct joint_info<prismatic_joint> {
    using type = dart::dynamics::PrismaticJoint;
    using props = type::Properties;

    static props create_props(const prismatic_joint& joint) {
        props props;
        return props;
    }

    static double get_pos(const type* joint) {
        return joint->getPosition(0);
    }
    static void set_pos(type* joint, const double value) {
        joint->setPosition(0, value);
    }
    static double get_vel(const type* joint) {
        return joint->getVelocity(0);
    }
    static void set_vel(type* joint, const double value) {
        joint->setVelocity(0, value);
    }
    static double get_accel(const type* joint) {
        return joint->getAcceleration(0);
    }
    static void set_accel(type* joint, const double value) {
        joint->setAcceleration(0, value);
    }
};

template<>
struct joint_info<ball_joint> {
    using type = dart::dynamics::BallJoint;
    using props = type::Properties;

    static props create_props(const ball_joint& joint) {
        props props;
        return props;
    }

    static Eigen::Vector3d get_pos(const type* joint) {
        return joint->getPositions();
    }
    static void set_pos(type* joint, const Eigen::Vector3d& value) {
        joint->setPositions(value);
    }
    static Eigen::Vector3d get_vel(const type* joint) {
        return joint->getVelocities();
    }
    static void set_vel(type* joint, const Eigen::Vector3d& value) {
        joint->setVelocities(value);
    }
    static Eigen::Vector3d get_accel(const type* joint) {
        return joint->getAccelerations();
    }
    static void set_accel(type* joint, const Eigen::Vector3d& value) {
        joint->setAccelerations(value);
    }
};

template<>
struct joint_info<free_joint> {
    using type = dart::dynamics::FreeJoint;
    using props = type::Properties;

    static props create_props(const free_joint& joint) {
        props props;
        return props;
    }

    static Eigen::Vector6d get_pos(const type* joint) {
        return joint->getPositions();
    }
    static void set_pos(type* joint, const Eigen::Vector6d& value) {
        joint->setPositions(value);
    }
    static Eigen::Vector6d get_vel(const type* joint) {
        return joint->getVelocities();
    }
    static void set_vel(type* joint, const Eigen::Vector6d& value) {
        joint->setVelocities(value);
    }
    static Eigen::Vector6d get_accel(const type* joint) {
        return joint->getAccelerations();
    }
    static void set_accel(type* joint, const Eigen::Vector6d& value) {
        joint->setAccelerations(value);
    }
};

template<joint_type joint_t_>
struct ligament_holder {
    using raw_joint_type = joint_info<joint_t_>::type;
    using raw_body_type = dart::dynamics::BodyNode;

    ligament_holder(raw_joint_type* joint, raw_body_type* body_node)
        : joint(joint)
        , body_node(body_node)
    {}

    raw_joint_type* joint;
    raw_body_type* body_node;
};

struct world_holder {
    explicit world_holder()
        : world(dart::simulation::World::create())
        , collision_group(){
        const auto bulletDetector = dart::collision::BulletCollisionDetector::create();
        world->getConstraintSolver()->setCollisionDetector(bulletDetector);
        collision_group = world->getConstraintSolver()->getCollisionDetector()->createCollisionGroupAsSharedPtr();
        world->setGravity(0, 0, 0);
    }

    dart::simulation::WorldPtr world;
    dart::collision::CollisionGroupPtr collision_group;
};

struct body_holder {
    explicit body_holder(const world_holder& world, const std::string_view name)
        : world(world)
        , skeleton(dart::dynamics::Skeleton::create(name.data()))
    {}

    world_holder world;
    dart::dynamics::SkeletonPtr skeleton;
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

inline body_holder create_body(const world_holder& world, const std::string_view name) {
    body_holder body(world, name);
    world.world->addSkeleton(body.skeleton);

    return body;
}

template<shape_type shape_t_, joint_type joint_t_>
ligament_holder<joint_t_> create_ligament(
    const body_holder& body,
    dart::dynamics::BodyNode* parent,
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

    auto [created_joint, body_node] =
            body.skeleton->createJointAndBodyNodePair<underlying_joint_type>(parent, joint_props, body_props);

    if constexpr (!std::is_same_v<shape_t_, empty_shape>) {
        auto shape_ptr = shape_info::create(shape);
        auto shape_node = body_node->createShapeNode(shape_ptr);
        create_aspects(shape_node, aspects);
    }

    body.world.collision_group->addShapeFramesOf(body_node);

    return {created_joint, body_node};
}

static visual_shape get_visual_shape(dart::dynamics::ShapeNode* node) {
    const auto& shape_type = node->getShape()->getType();
    if (shape_type == dart::dynamics::BoxShape::getStaticType()) {
        return visual_shape::box;
    }
    if (shape_type == dart::dynamics::SphereShape::getStaticType()) {
        return visual_shape::sphere;
    }

    return visual_shape::unknown;
}

inline void world_step(const world_holder& holder) {
    holder.world->step();
}

using render_function = std::function<void(const Eigen::Matrix4d&, visual_shape)>;
void world_render(const world_holder& world, render_function&& render_action);
std::optional<raycast_result> raycast(const world_holder& world, const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, units::meters min_distance, units::meters max_distance);

}
