#pragma once

#include <ranges>
#include <dart/dart.hpp>

#include "dynamics/data.h"

namespace autobot::sim::dynamics {

enum class visual_shape {
    unknown,
    box,
    sphere,
};

template<shape_type>
struct shape_info {};

template<>
struct shape_info<box_shape> {
    using shape = dart::dynamics::BoxShape;
};

template<>
struct shape_info<sphere_shape> {
    using shape = dart::dynamics::SphereShape;
};

template<>
struct shape_info<cylinder_shape> {
    using shape = dart::dynamics::CylinderShape;
};

template<>
struct shape_info<ellipsoid_shape> {
    using shape = dart::dynamics::EllipsoidShape;
};

template<joint_type>
struct joint_info {};

template<>
struct joint_info<weld_joint> {
    using type = dart::dynamics::WeldJoint;
    using props = type::Properties;
};

template<>
struct joint_info<revolute_joint> {
    using type = dart::dynamics::RevoluteJoint;
    using props = type::Properties;

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
        : joint(joint), body_node(body_node)
    {}

    ligament_holder(const ligament_holder&) = default;
    ligament_holder(ligament_holder&&) = default;
    ligament_holder& operator=(const ligament_holder&) = default;
    ligament_holder& operator=(ligament_holder&&) = default;

    raw_joint_type* joint;
    raw_body_type* body_node;
};

struct body_holder {
    explicit body_holder(const std::string_view name)
        : skeleton(dart::dynamics::Skeleton::create(name.data()))
    {}

    dart::dynamics::SkeletonPtr skeleton;
};

struct world_holder {
    explicit world_holder()
        : world(dart::simulation::World::create()) {
        world->setGravity(0, 0, 0);
    }

    dart::simulation::WorldPtr world;
};

template<joint_type joint_t_>
auto create_joint_properties(const joint_t_& joint) {
    if constexpr (std::is_same_v<joint_t_, weld_joint>) {
        dart::dynamics::WeldJoint::Properties props;
        return props;
    } else if constexpr (std::is_same_v<joint_t_, revolute_joint>) {
        dart::dynamics::RevoluteJoint::Properties props;
        props.mAxis = joint.rotation_axis;
        return props;
    } else if constexpr (std::is_same_v<joint_t_, prismatic_joint>) {
        dart::dynamics::PrismaticJoint::Properties props;
        return props;
    } else if constexpr (std::is_same_v<joint_t_, ball_joint>) {
        dart::dynamics::BallJoint::Properties props;
        return props;
    } else if constexpr (std::is_same_v<joint_t_, free_joint>) {
        dart::dynamics::FreeJoint::Properties props;
        return props;
    } else {
        static_assert(false, "unknown joint");
    }
}

template<shape_type shape_t_>
auto create_shape(const shape_t_& shape) {
    if constexpr (std::is_same_v<shape_t_, empty_shape>) {
        return std::shared_ptr<dart::dynamics::BoxShape>();
    } else if constexpr (std::is_same_v<shape_t_, box_shape>) {
        return std::make_shared<dart::dynamics::BoxShape>(shape.dims());
    } else if constexpr (std::is_same_v<shape_t_, sphere_shape>) {
        return std::make_shared<dart::dynamics::SphereShape>(shape.radius.value());
    } else if constexpr (std::is_same_v<shape_t_, cylinder_shape>) {
        return std::make_shared<dart::dynamics::CylinderShape>(shape.radius.value(), shape.height.value());
    } else if constexpr (std::is_same_v<shape_t_, ellipsoid_shape>) {
        return std::make_shared<dart::dynamics::EllipsoidShape>(shape.dims());
    } else {
        static_assert(false, "unknown shape");
    }
}

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
ligament_holder<joint_t_> create_ligament(
    const body_holder& body_holder,
    dart::dynamics::BodyNode* parent,
    const std::string_view name,
    const shape_t_& shape,
    const joint_t_& joint,
    const Eigen::Isometry3d& transform,
    const ligament_aspect aspects) {
    using underlying_joint_type = joint_info<joint_t_>::type;

    auto joint_props = create_joint_properties(joint);
    joint_props.mName = name;
    joint_props.mT_ParentBodyToJoint = transform;

    dart::dynamics::BodyNode::Properties body_props;
    body_props.mName = name;

    auto [created_joint, body_node] =
            body_holder.skeleton->createJointAndBodyNodePair<underlying_joint_type>(parent, joint_props, body_props);

    if (auto shape_ptr = create_shape(shape); shape_ptr) {
        auto shape_node = body_node->createShapeNode(shape_ptr);
        create_aspects(shape_node, aspects);
    }

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

inline void world_add(const world_holder& holder, const body_holder& body_holder) {
    holder.world->addSkeleton(body_holder.skeleton);
}

inline void world_step(const world_holder& holder) {
    holder.world->step();
}

inline void world_render(const world_holder& holder, std::function<void(const Eigen::Matrix4d&, visual_shape)>&& render_action) {
    for (size_t i = 0; i < holder.world->getNumSkeletons(); ++i) {
        const auto skeleton = holder.world->getSkeleton(i);
        for (size_t j = 0; j < skeleton->getNumBodyNodes(); ++j) {
            auto* body_node = skeleton->getBodyNode(j);

            Eigen::Isometry3d body_tf = body_node->getWorldTransform();
            for (size_t k = 0; k < body_node->getNumShapeNodes(); ++k) {
                auto* shape_node = body_node->getShapeNode(k);
                auto total_tf = body_tf * shape_node->getRelativeTransform().matrix();

                bool draw = true;
                const auto vis_shape = get_visual_shape(shape_node);
                switch (vis_shape) {
                    case visual_shape::box: {
                        const auto* box = static_cast<dart::dynamics::BoxShape*>(shape_node->getShape().get());
                        const auto size = box->getSize();
                        Eigen::DiagonalMatrix<double, 4> scaling(size.x(), size.y(), size.z(), 1);
                        total_tf = total_tf * scaling;
                        break;
                    }
                    case visual_shape::sphere:
                        break;
                    case visual_shape::unknown:
                    default:
                        draw = false;
                        break;
                }

                if (!draw) {
                    continue;
                }

                render_action(total_tf, vis_shape);
            }
        }
    }
}

}
