#pragma once

#include <functional>
#include <Eigen/Dense>

#include "autobot_sim/dynamics/data.h"

namespace autobot::sim::dynamics::engine {

enum class visual_shape {
    unknown,
    box,
    sphere,
};

struct world_holder {
    world_holder() = default;
    virtual ~world_holder() = default;
};

using world_holder_ptr = std::shared_ptr<world_holder>;

world_holder_ptr create_world();

void step_world(const world_holder_ptr& world);

using render_function = std::function<void(const Eigen::Matrix4d&, visual_shape)>;
void render_world(const world_holder_ptr& world, render_function&& render_action);

}
