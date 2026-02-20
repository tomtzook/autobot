find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_library(imgui STATIC
        # Main Imgui files
        libs/imgui/imgui.cpp
        libs/imgui/imgui.h
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/imgui_demo.cpp
        # backend-specific files
        libs/imgui/backends/imgui_impl_glfw.cpp
        libs/imgui/backends/imgui_impl_glfw.h
        libs/imgui/backends/imgui_impl_opengl3.cpp
        libs/imgui/backends/imgui_impl_opengl3.h)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/ ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends/)

add_library(implot STATIC
        libs/implot/implot.cpp
        libs/implot/implot_items.cpp)
target_include_directories(implot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/implot/)
target_link_libraries(implot imgui)

add_executable(autoboard src/main.cpp
        src/plots/xy_plot.h
        src/plots/xy_plot.cpp
        src/plots/timed_plot_updater.h
        src/plots/plots.h
        src/plots/plots.cpp
        src/container.h
        src/container.cpp
        src/widgets/widgets.h
        src/widgets/canvas.h
        src/data/source.h
        src/data/source.cpp
        src/data/scheme.h
        src/data/registry.h
        src/data/registry.cpp
        src/data/obsr.h
        src/data/obsr.cpp
        src/data/obsr_storage.h
        src/data/obsr_storage.cpp
        src/widgets/obsr.h
        src/widgets/obsr.cpp
        src/widgets/canvas.cpp
        src/render/mesh.h
        src/render/mesh.cpp
        src/glad/glad.c
        src/render/gl.h
        src/render/gl.cpp
        src/render/shader.h
        src/render/shader.cpp
        src/render/render.cpp
        src/render/render.h
        src/render/camera.h
        src/render/camera.cpp
)
target_include_directories(autoboard PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(autoboard imgui implot Threads::Threads fmt::fmt looper::looper obsr::obsr OpenGL::GL glfw glm::glm)
