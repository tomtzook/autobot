#pragma once

#include <functional>

#include "autobot_sim/dynamics/engine/base.h"

namespace autobot::sim::dynamics::engine {

struct world_holder {
    explicit world_holder()
        : world(dart::simulation::World::create())
        , collision_group() {
        const auto bulletDetector = dart::collision::BulletCollisionDetector::create();
        world->getConstraintSolver()->setCollisionDetector(bulletDetector);
        collision_group = world->getConstraintSolver()->getCollisionDetector()->createCollisionGroupAsSharedPtr();
        world->setGravity(0, 0, 0);
    }

    dart::simulation::WorldPtr world;
    dart::collision::CollisionGroupPtr collision_group;
};

void step_world(const world_holder& world);

using render_function = std::function<void(const Eigen::Matrix4d&, visual_shape)>;
void render_world(const world_holder& world, render_function&& render_action);

}
