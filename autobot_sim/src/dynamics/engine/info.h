#pragma once

#include <dart/dart.hpp>
#include <dart/collision/bullet/BulletCollisionDetector.hpp>

#include "autobot_sim/dynamics/data.h"

namespace autobot::sim::dynamics::engine {

template<shape_type>
struct dart_shape_info {};

template<>
struct dart_shape_info<box_shape> {
    using type = dart::dynamics::BoxShape;
    using type_ptr = std::shared_ptr<type>;

    static type_ptr create(const box_shape& shape) {
        return std::make_shared<type>(shape.dims());
    }
};

template<>
struct dart_shape_info<sphere_shape> {
    using type = dart::dynamics::SphereShape;
    using type_ptr = std::shared_ptr<type>;

    static type_ptr create(const sphere_shape& shape) {
        return std::make_shared<type>(shape.radius.value());
    }
};

template<>
struct dart_shape_info<cylinder_shape> {
    using type = dart::dynamics::CylinderShape;
    using type_ptr = std::shared_ptr<type>;

    static type_ptr create(const cylinder_shape& shape) {
        return std::make_shared<type>(shape.radius.value(), shape.height.value());
    }
};

template<>
struct dart_shape_info<ellipsoid_shape> {
    using type = dart::dynamics::EllipsoidShape;
    using type_ptr = std::shared_ptr<type>;

    static type_ptr create(const ellipsoid_shape& shape) {
        return std::make_shared<type>(shape.dims());
    }
};

template<joint_type>
struct dart_joint_info {};

template<>
struct dart_joint_info<weld_joint> {
    using type = dart::dynamics::WeldJoint;
    using props = type::Properties;

    static props create_props(const weld_joint& joint) {
        props props;
        return props;
    }
};

template<>
struct dart_joint_info<revolute_joint> {
    using type = dart::dynamics::RevoluteJoint;
    using props = type::Properties;

    static props create_props(const revolute_joint& joint) {
        props props;
        props.mAxis = joint.rotation_axis.raw();

        switch (joint.actuator) {
            case actuator_type::force:
                props.mActuatorType = dart::dynamics::detail::FORCE;
                break;
            case actuator_type::none:
            default:
                props.mActuatorType = dart::dynamics::detail::PASSIVE;
                break;
        }

        return props;
    }
};

template<>
struct dart_joint_info<prismatic_joint> {
    using type = dart::dynamics::PrismaticJoint;
    using props = type::Properties;

    static props create_props(const prismatic_joint& joint) {
        props props;
        return props;
    }
};

template<>
struct dart_joint_info<ball_joint> {
    using type = dart::dynamics::BallJoint;
    using props = type::Properties;

    static props create_props(const ball_joint& joint) {
        props props;
        return props;
    }
};

template<>
struct dart_joint_info<free_joint> {
    using type = dart::dynamics::FreeJoint;
    using props = type::Properties;

    static props create_props(const free_joint& joint) {
        props props;
        return props;
    }
};

template<joint_type joint_t_>
using raw_joint_type = dart_joint_info<joint_t_>::type*;

}
