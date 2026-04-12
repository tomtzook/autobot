
#include "autobot_sim/dynamics/engine/raycast.h"

namespace autobot::sim::dynamics::engine {

std::optional<double> raycast(const world_holder& world, const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, const double max_distance) {
    return raycast(world, origin, direction, 0, max_distance);
}

std::optional<double> raycast(const world_holder& world, const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, const double min_distance, const double max_distance) {
    const auto to = origin + (direction * max_distance).eval();

    dart::collision::RaycastOption option;
    option.mEnableAllHits = false;
    option.mSortByClosest = true;
    dart::collision::RaycastResult result;
    if (world.collision_group->raycast(origin, to, option, &result)) {
        const auto& first_hit = result.mRayHits[0];
        const auto hit_point = first_hit.mPoint;
        const auto hit_distance = (origin - hit_point).norm();
        if (hit_distance >= min_distance) {
            return hit_distance;
        }
    }

    return std::nullopt;
}

}
