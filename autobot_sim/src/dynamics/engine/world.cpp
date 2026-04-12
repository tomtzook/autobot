
#include "autobot_sim/dynamics/engine/world.h"

namespace autobot::sim::dynamics::engine {

static visual_shape get_visual_shape(dart::dynamics::ShapeNode* node) {
    const auto& shape_type = node->getShape()->getType();
    if (shape_type == dart::dynamics::BoxShape::getStaticType()) {
        return visual_shape::box;
    }
    if (shape_type == dart::dynamics::SphereShape::getStaticType()) {
        return visual_shape::sphere;
    }

    return visual_shape::unknown;
}

void step_world(const world_holder& world) {
    world.world->step();
}

void render_world(const world_holder& world, render_function&& render_action) {
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

}
