
#include "autobot_sim/dynamics/engine.h"

namespace autobot::sim::dynamics::engine {

void world_render(const world_holder& world, render_function&& render_action) {
    for (size_t i = 0; i < world.world->getNumSkeletons(); ++i) {
        const auto skeleton = world.world->getSkeleton(i);
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

std::optional<raycast_result> raycast(const world_holder& world, const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, const units::meters min_distance, const units::meters max_distance) {
    const auto to = origin + (direction * max_distance.value()).eval();

    dart::collision::RaycastOption option;
    option.mEnableAllHits = false;
    option.mSortByClosest = true;
    dart::collision::RaycastResult result;
    if (world.collision_group->raycast(origin, to, option, &result)) {
        const auto& first_hit = result.mRayHits[0];
        const auto hit_point = first_hit.mPoint;
        const auto hit_distance = units::meters((origin - hit_point).norm());
        if (hit_distance >= min_distance) {
            return raycast_result{hit_distance, hit_point};
        }
    }

    return std::nullopt;
}

}
