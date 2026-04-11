#pragma once

#include <Eigen/Core>
#include <Eigen/src/Core/Matrix.h>
#include <Eigen/src/Geometry/AngleAxis.h>
#include <Eigen/src/Geometry/Quaternion.h>
#include <Eigen/src/Geometry/Transform.h>

#include <autobot/units.h>

namespace autobot::sim {

inline Eigen::Isometry3d transform(const Eigen::Vector3d& translation, const Eigen::Matrix3d& rotation) {
    auto transform = Eigen::Isometry3d::Identity();
    transform.translation() = translation;
    transform.linear() = rotation;
    return transform;
}

inline Eigen::Isometry3d transform(const Eigen::Vector3d& translation, const Eigen::Quaterniond& rotation) {
    const auto rot_mat = rotation.toRotationMatrix();
    return transform(translation, rot_mat);
}

inline Eigen::Isometry3d transform(
    const units::meters x, const units::meters y, const units::meters z,
    const units::radians roll, const units::radians pitch, const units::radians yaw) {
    const auto translation = Eigen::Vector3d{x.value(), y.value(), z.value()};
    const auto rotation =
        Eigen::AngleAxisd(roll.value(),Eigen::Vector3d::UnitX()) *
        Eigen::AngleAxisd(pitch.value(),Eigen::Vector3d::UnitY()) *
        Eigen::AngleAxisd(yaw.value(),Eigen::Vector3d::UnitZ());

    return transform(translation, rotation);
}

}
