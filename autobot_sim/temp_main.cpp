
#include <cstdio>
#include <unistd.h>

#include <glad/glad.h>
#include <Eigen/Core>
#include <dart/dart.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glui/render.h>
#include <glui/camera.h>
#include <glui/window.h>

#include <units.h>

#include "dynamics/body.h"
#include "transform.h"


static std::optional<glui::mesh> g_cube_mesh;

int main() {
    glui::window window("Main Window", 1280, 720, glm::vec4(0.45f, 0.55f, 0.60f, 1.00f));

    g_cube_mesh = glui::cube_mesh(1, 1, 1);
    g_cube_mesh->color(glui::gl::color(34, 0, 140));

    glui::renderer renderer;
    glui::camera camera(glm::radians(45.0f), 1280.0f / 720.0f, 0.01f, 100.0f);
    window.on_update([&camera, &window]()->void {
        if (window.get_key(GLFW_KEY_W) == GLFW_PRESS) {
            camera.move_forward(0.1);
        }
        if (window.get_key(GLFW_KEY_S) == GLFW_PRESS) {
            camera.move_backward(0.1);
        }
        if (window.get_key(GLFW_KEY_D) == GLFW_PRESS) {
            camera.move_right(0.1);
        }
        if (window.get_key(GLFW_KEY_A) == GLFW_PRESS) {
            camera.move_left(0.1);
        }
        if (window.get_key(GLFW_KEY_UP) == GLFW_PRESS) {
            camera.rotate_pitch(glm::radians(1.0f));
        }
        if (window.get_key(GLFW_KEY_DOWN) == GLFW_PRESS) {
            camera.rotate_pitch(glm::radians(-1.0f));
        }
        if (window.get_key(GLFW_KEY_RIGHT) == GLFW_PRESS) {
            camera.rotate_yaw(glm::radians(-1.0f));
        }
        if (window.get_key(GLFW_KEY_LEFT) == GLFW_PRESS) {
            camera.rotate_yaw(glm::radians(1.0f));
        }
    }, 0.02);

    autobot::sim::dynamics::world world;
    window.on_update([&world]()->void {
        world.step();
    }, 0.001);
    window.on_render([&renderer, &world, &camera]()->void {
        auto render_context = renderer.start(camera.projection(), camera.view());

        // dart uses Z-axis as UP, while opengl uses Y-axis as up
        Eigen::Isometry3d dartToOpengl = Eigen::Isometry3d::Identity();
        dartToOpengl.linear() = Eigen::AngleAxisd(-M_PI/2, Eigen::Vector3d::UnitX()).toRotationMatrix();

        world.render([&render_context, &dartToOpengl](const auto& tf, const auto shape)->void {
            glui::mesh* mesh_ptr = nullptr;
            switch (shape) {
                case autobot::sim::dynamics::visual_shape::box:
                    mesh_ptr = &g_cube_mesh.value();
                    break;
                default:
                    fprintf(stderr, "Unknown shape to draw\n");
                    std::abort();
            }

            Eigen::Matrix4f model_matrix = (dartToOpengl * tf).template cast<float>();
            render_context.render(glm::make_mat4(model_matrix.data()), *mesh_ptr);
        });
    });

    autobot::sim::dynamics::container robot1("robot1");
    world.add(robot1);

    using namespace autobot::units::literals;
    auto ultrasonic_ligament = robot1.attach("ultrasonic",
        autobot::sim::dynamics::box_shape(0.45_m, 0.15_m, 0.2_m),
        autobot::sim::dynamics::revolute_joint{.rotation_axis = Eigen::Vector3d::UnitZ()},
        Eigen::Isometry3d::Identity(),
        autobot::sim::dynamics::ligament_aspect::all);
    auto ligament2 = robot1.attach("lig2",
        autobot::sim::dynamics::box_shape(0.1_m, 0.1_m, 0.3_m),
        autobot::sim::dynamics::weld_joint{},
        autobot::sim::transform(0.2_m, 0.1_m, 0.1_m, 0.0_rad, 1.0_rad, 0.0_rad),
        autobot::sim::dynamics::ligament_aspect::all);

    window.on_update([&window, &ultrasonic_ligament]()->void {
        if (window.get_key(GLFW_KEY_T) == GLFW_PRESS) {
            auto pos = ultrasonic_ligament.get_position();
            pos += 1.0_rad;
            ultrasonic_ligament.set_position(pos);
        }
        if (window.get_key(GLFW_KEY_R) == GLFW_PRESS) {
            auto pos = ultrasonic_ligament.get_position();
            pos -= 1.0_rad;
            ultrasonic_ligament.set_position(pos);
        }
    }, 0.02);

    while (window.iterate()) {}

    return 0;
}
