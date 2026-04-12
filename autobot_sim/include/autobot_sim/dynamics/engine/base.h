#pragma once

#include <dart/dart.hpp>
#include <dart/collision/bullet/BulletCollisionDetector.hpp>

#include "autobot_sim/dynamics/data.h"

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

struct body_node {
    using type = dart::dynamics::BodyNode;

    static Eigen::Isometry3d get_world_transform(const type* type) {
        return type->getWorldTransform();
    }

    static Eigen::Vector3d get_world_position(const type* type) {
        return get_world_transform(type).translation();
    }

    static Eigen::Quaterniond get_world_rotation(const type* type) {
        const auto transform = get_world_transform(type);
        auto quat = Eigen::Quaterniond(transform.linear());
        quat.normalize();
        return quat;
    }

    static Eigen::Vector3d get_world_linear_velocity(const type* type) {
        return type->getLinearVelocity();
    }

    static Eigen::Vector3d get_world_angular_velocity(const type* type) {
        return type->getAngularVelocity();
    }

    static Eigen::Vector3d get_world_linear_acceleration(const type* type) {
        return type->getLinearAcceleration();
    }

    static Eigen::Vector3d get_world_angular_acceleration(const type* type) {
        return type->getAngularAcceleration();
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
        props.mAxis = joint.rotation_axis.raw();
        return props;
    }

    static double get_position(const type* joint) {
        return joint->getPosition(0);
    }

    static void set_position(type* joint, const double value) {
        joint->setPosition(0, value);
    }

    static double get_velocity(const type* joint) {
        return joint->getVelocity(0);
    }

    static void set_velocity(type* joint, const double value) {
        joint->setVelocity(0, value);
    }

    static double get_acceleration(const type* joint) {
        return joint->getAcceleration(0);
    }

    static void set_acceleration(type* joint, const double value) {
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

    static double get_position(const type* joint) {
        return joint->getPosition(0);
    }

    static void set_position(type* joint, const double value) {
        joint->setPosition(0, value);
    }

    static double get_velocity(const type* joint) {
        return joint->getVelocity(0);
    }

    static void set_velocity(type* joint, const double value) {
        joint->setVelocity(0, value);
    }

    static double get_acceleration(const type* joint) {
        return joint->getAcceleration(0);
    }

    static void set_acceleration(type* joint, const double value) {
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

    static Eigen::Vector3d get_position(const type* joint) {
        return joint->getPositions();
    }

    static void set_position(type* joint, const Eigen::Vector3d& value) {
        joint->setPositions(value);
    }

    static Eigen::Vector3d get_velocity(const type* joint) {
        return joint->getVelocities();
    }

    static void set_velocity(type* joint, const Eigen::Vector3d& value) {
        joint->setVelocities(value);
    }

    static Eigen::Vector3d get_acceleration(const type* joint) {
        return joint->getAccelerations();
    }

    static void set_acceleration(type* joint, const Eigen::Vector3d& value) {
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

    static Eigen::Vector3d get_linear_position(const type* joint) {
        return joint->getPositions().segment<3>(3);
    }

    static void set_linear_position(type* joint, const Eigen::Vector3d& value) {
        auto data = joint->getPositions();
        data.segment<3>(3) = value;
        joint->setPositions(data);
    }

    static Eigen::Vector3d get_linear_velocity(const type* joint) {
        return joint->getVelocities().segment<3>(3);
    }

    static void set_linear_velocity(type* joint, const Eigen::Vector3d& value) {
        auto data = joint->getVelocities();
        data.segment<3>(3) = value;
        joint->setVelocities(data);
    }

    static Eigen::Vector3d get_linear_acceleration(const type* joint) {
        return joint->getAccelerations().segment<3>(3);
    }

    static void set_linear_acceleration(type* joint, const Eigen::Vector3d& value) {
        auto data = joint->getAccelerations();
        data.segment<3>(3) = value;
        joint->setAccelerations(data);
    }

    static Eigen::Vector3d get_angular_position(const type* joint) {
        return joint->getPositions().segment<3>(0);
    }

    static void set_angular_position(type* joint, const Eigen::Vector3d& value) {
        auto data = joint->getPositions();
        data.segment<3>(0) = value;
        joint->setPositions(data);
    }

    static Eigen::Vector3d get_angular_velocity(const type* joint) {
        return joint->getVelocities().segment<3>(0);
    }

    static void set_angular_velocity(type* joint, const Eigen::Vector3d& value) {
        auto data = joint->getVelocities();
        data.segment<3>(0) = value;
        joint->setVelocities(data);
    }

    static Eigen::Vector3d get_angular_acceleration(const type* joint) {
        return joint->getAccelerations().segment<3>(0);
    }

    static void set_angular_acceleration(type* joint, const Eigen::Vector3d& value) {
        auto data = joint->getAccelerations();
        data.segment<3>(0) = value;
        joint->setAccelerations(data);
    }
};

using raw_body_node_type = body_node::type*;
template<joint_type joint_t_>
using raw_joint_type = joint_info<joint_t_>::type*;
using raw_body_type = dart::dynamics::SkeletonPtr;

}
