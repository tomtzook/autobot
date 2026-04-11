
add_library(glui STATIC
    src/gl.cpp
    src/mesh.cpp
    src/render.cpp
    src/shader.cpp
    src/camera.cpp
    src/glad/glad.c
        include/glui/window.h
        src/window.cpp
)
target_include_directories(glui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(glui PUBLIC OpenGL::GL glfw glm::glm)
