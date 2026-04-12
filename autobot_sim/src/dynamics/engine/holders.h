#pragma once

#include "autobot_sim/dynamics/engine/skeleton.h"
#include "autobot_sim/dynamics/engine/world.h"
#include "info.h"

namespace autobot::sim::dynamics::engine {

struct dart_world_holder final : world_holder {
    explicit dart_world_holder();

    dart_world_holder(const dart_world_holder&) = delete;
    dart_world_holder(dart_world_holder&&) = default;
    dart_world_holder& operator=(const dart_world_holder&) = delete;
    dart_world_holder& operator=(dart_world_holder&&) = default;

    dart::simulation::WorldPtr world;
    dart::collision::CollisionGroupPtr collision_group;
};

struct dart_base_ligament_holder : ligament_holder {
    dart_base_ligament_holder(const world_holder_ptr& world, const body_holder_ptr& body, dart::dynamics::BodyNode* node);

    dart_base_ligament_holder(const dart_base_ligament_holder&) = delete;
    dart_base_ligament_holder(dart_base_ligament_holder&&) = default;
    dart_base_ligament_holder& operator=(const dart_base_ligament_holder&) = delete;
    dart_base_ligament_holder& operator=(dart_base_ligament_holder&&) = default;

    world_holder_ptr world;
    body_holder_ptr body;
    dart::dynamics::BodyNode* node;
};

template<joint_type joint_t_>
struct dart_ligament_holder final : dart_base_ligament_holder {
    dart_ligament_holder(const world_holder_ptr& world, const body_holder_ptr& body, dart::dynamics::BodyNode* node, raw_joint_type<joint_t_> joint)
        : dart_base_ligament_holder(world, body, node)
        , joint(joint)
    {}

    dart_ligament_holder(const dart_ligament_holder&) = delete;
    dart_ligament_holder(dart_ligament_holder&&) = default;
    dart_ligament_holder& operator=(const dart_ligament_holder&) = delete;
    dart_ligament_holder& operator=(dart_ligament_holder&&) = default;

    raw_joint_type<joint_t_> joint;
};

struct dart_body_holder final : body_holder {
    dart_body_holder(const world_holder_ptr& world, const dart::dynamics::SkeletonPtr& body);

    dart_body_holder(const dart_body_holder&) = delete;
    dart_body_holder(dart_body_holder&&) = default;
    dart_body_holder& operator=(const dart_body_holder&) = delete;
    dart_body_holder& operator=(dart_body_holder&&) = default;

    world_holder_ptr world;
    dart::dynamics::SkeletonPtr body;
};

inline dart_world_holder& holder(const world_holder_ptr& ptr) {
    return reinterpret_cast<dart_world_holder&>(*ptr.get());
}

inline dart_base_ligament_holder& holder(const ligament_holder_ptr& ptr) {
    return reinterpret_cast<dart_base_ligament_holder&>(*ptr.get());
}

template<joint_type joint_t_>
dart_ligament_holder<joint_t_>& holder_joint(const ligament_holder_ptr& ptr) {
    return reinterpret_cast<dart_ligament_holder<joint_t_>&>(*ptr.get());
}

inline dart_body_holder& holder(const body_holder_ptr& ptr) {
    return reinterpret_cast<dart_body_holder&>(*ptr.get());
}

}
