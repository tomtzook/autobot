
#include "autobot_sim/dynamics/engine/skeleton.h"

namespace autobot::sim::dynamics::engine {

body_holder create_body(const world_holder& world, const std::string_view name) {
    auto skeleton = dart::dynamics::Skeleton::create(name.data());
    auto root_ligament = create_root_ligament(world, skeleton, "root", empty_shape{}, free_joint{}, Eigen::Isometry3d::Identity(), ligament_aspect::all);

    body_holder body{world, std::move(skeleton), std::move(root_ligament)};
    world.world->addSkeleton(body.body);

    return body;
}

}
