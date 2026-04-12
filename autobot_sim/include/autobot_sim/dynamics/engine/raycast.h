#pragma once

#include "autobot_sim/dynamics/engine/world.h"

namespace autobot::sim::dynamics::engine {

std::optional<double> raycast(const world_holder_ptr& world, const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, double max_distance);
std::optional<double> raycast(const world_holder_ptr& world, const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, double min_distance, double max_distance);

}
