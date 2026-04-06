
#include "dynamics/data.h"

namespace autobot::sim::dynamics {

box_shape::box_shape(const units::meters length, const units::meters width, const units::meters height)
    : length(length)
    , width(width)
    , height(height)
{}

Eigen::Vector3d box_shape::dims() const {
    return {length.value(), width.value(), height.value()};
}

sphere_shape::sphere_shape(const units::meters radius)
    : radius(radius)
{}

cylinder_shape::cylinder_shape(const units::meters radius, const units::meters height)
    : radius(radius)
    , height(height)
{}


ellipsoid_shape::ellipsoid_shape(const units::meters diameter_x, const units::meters diameter_y, const units::meters diameter_z)
    : diameter_x(diameter_x)
    , diameter_y(diameter_y)
    , diameter_z(diameter_z)
{}

Eigen::Vector3d ellipsoid_shape::dims() const {
    return Eigen::Vector3d{diameter_x.value(), diameter_y.value(), diameter_z.value()};
}

}
