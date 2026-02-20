
#include <chrono>
#include <iostream>

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <GLFW/glfw3.h>
#include <implot.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "render/render.h"
#include "render/camera.h"
#include "container.h"


static void glfw_error_callback(const int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static glm::mat4 transformation(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * glm::mat4_cast(rotation);
    model = glm::scale(model, scale);
    return model;
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    auto glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Main Window", nullptr, nullptr);
    if (window == nullptr)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImPlot::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    constexpr auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ui::container container;
    ui::render::renderer renderer3d;
    ui::render::camera camera(glm::radians(45.0f), 1280.0f / 720.0f, 0.01f, 100.0f, 1.0f, 0.5f);

    auto cube = ui::render::cube_mesh(1, 1, 1);
    cube.color(gl::color(255, 0, 0, 255));
    const auto transformation_cube = transformation(glm::vec3(0.1f, 0.0f, -3.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f));

    auto last_update_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());;
    while (!glfwWindowShouldClose(window)) {
        // poll events
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // create frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            (now - last_update_time) >= std::chrono::milliseconds(20)) {
            last_update_time = now;
            container.update();
        }

        container.draw();

        // render
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            auto renderer = renderer3d.start(camera.projection(), camera.view());
            renderer.render(transformation_cube, cube);
        }

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
