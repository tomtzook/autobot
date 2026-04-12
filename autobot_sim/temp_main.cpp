
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

#include <hal.h>
#include <hal_sim.h>
#include <autobot/units.h>

#include "autobot_sim/dynamics/body.h"
#include "autobot_sim/devices/hcsr04.h"


static std::optional<glui::mesh> g_cube_mesh;

int main() {
    using namespace autobot::units::literals;

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
        //Eigen::Isometry3d dartToOpengl = Eigen::Isometry3d::Identity();
        //dartToOpengl.linear() = Eigen::AngleAxisd(-M_PI/2, Eigen::Vector3d::UnitX()).toRotationMatrix();
        Eigen::Matrix3d R_d2g;
        R_d2g << 0, -1,  0,  // OpenGL X is DART -Y
                 0,  0,  1,  // OpenGL Y is DART Z
                 1,  0,  0;  // OpenGL Z is DART X

        world.render([&render_context](const Eigen::Matrix4d& tf, const auto shape)->void {
            glui::mesh* mesh_ptr = nullptr;
            switch (shape) {
                case autobot::sim::dynamics::engine::visual_shape::box:
                    mesh_ptr = &g_cube_mesh.value();
                    break;
                default:
                    fprintf(stderr, "Unknown shape to draw\n");
                    std::abort();
            }

            Eigen::Matrix4d S = Eigen::Matrix4d::Zero();
            S(0, 1) = -1.0; // Row 0 (GL X) is -Col 1 (DART Y)
            S(1, 2) =  1.0; // Row 1 (GL Y) is Col 2 (DART Z)
            S(2, 0) =  -1.0; // Row 2 (GL Z) is Col 0 (DART X)
            S(3, 3) =  1.0; // Keep the homogeneous w-component

            //Eigen::Matrix4f model_matrix = (dartToOpengl * tf).template cast<float>();
            Eigen::Matrix4d glT = S * tf * S.transpose();
            Eigen::Matrix4f model_matrix = glT.cast<float>();
            render_context.render(glm::make_mat4(model_matrix.data()), *mesh_ptr);
        });
    });

    autobot::hal::initialize(autobot::hal::sim::initialize);
    autobot::hal::sim::define(1, "trig", autobot::hal::type_port_digital_output);
    autobot::hal::sim::define_value(1, autobot::hal::value_digital_io_signal, "iosig", autobot::hal::type_port_digital_output, autobot::hal::data_type::unsigned_32bit, autobot::hal::data_permission::readwrite, autobot::hal::value_capabilities::pulse);
    autobot::hal::sim::define(2, "echo", autobot::hal::type_pulsewidth_reader);
    autobot::hal::sim::define_value(2, autobot::hal::value_pulsewidth_length, "pulse", autobot::hal::type_pulsewidth_reader, autobot::hal::data_type::unsigned_32bit, autobot::hal::data_permission::readonly);

    auto robot1 = world.create_body("robot1");
    /*auto ultrasonic_ligament = robot1.attach("ultrasonic",
        autobot::sim::hcsr04::shape,
        autobot::sim::dynamics::revolute_joint{.rotation_axis = Eigen::Vector3d::UnitZ()},
        Eigen::Isometry3d::Identity(),
        autobot::sim::dynamics::ligament_aspect::all);*/

    autobot::sim::devices::hcsr04 hcsr04_sim(1, 2);
    auto ultrasonic_joint = robot1.attach("ultrasonic", hcsr04_sim, autobot::sim::dynamics::revolute_joint{.rotation_axis = autobot::math::axis_z()});
    auto echo_reader = autobot::hal::pulse_width_reader(autobot::hal::open_device(2, autobot::hal::type_pulsewidth_reader));

    auto robot2 = world.create_body("robot2");
    //auto robot2_lig1 = robot2.weld("lig1", )

    /*auto robot2_lig1 = robot1.attach("lig1",
        autobot::sim::dynamics::box_shape(0.45_m, 0.15_m, 0.2_m),
        autobot::sim::dynamics::weld_joint{},
        autobot::sim::transform(1.0_m, 0.0_m, 0.0_m, 0.0_rad, 0.0_rad, 0.0_rad),
        autobot::sim::dynamics::ligament_aspect::all);*/

    window.on_update([&window, &ultrasonic_joint, &hcsr04_sim, &echo_reader]()->void {
        if (window.get_key(GLFW_KEY_T) == GLFW_PRESS) {
            auto pos = ultrasonic_joint.get_position();
            pos += 1.0_deg;
            ultrasonic_joint.set_position(pos);
        }
        if (window.get_key(GLFW_KEY_R) == GLFW_PRESS) {
            auto pos = ultrasonic_joint.get_position();
            pos -= 1.0_deg;
            ultrasonic_joint.set_position(pos);
        }
        if (window.get_key(GLFW_KEY_Y) == GLFW_PRESS) {
            hcsr04_sim.measure();

            const auto length = echo_reader.read();
            printf("LEN: at %u\n", length);
        }
    }, 0.02);

    while (window.iterate()) {}

    return 0;
}
