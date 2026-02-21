
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mesh.h"

namespace ui::render {

mesh::mesh(
    const std::span<const float> positions,
    const std::span<const float> tex_coords,
    const std::span<const float> normals,
    const std::span<const unsigned int> indices)
        : m_positions()
        , m_tex_coords()
        , m_normals()
        , m_indices()
        , m_draw_count(indices.size())
        , m_color() {
    m_positions.set(gl::target::array_buffer, gl::usage::static_draw, positions);
    m_tex_coords.set(gl::target::array_buffer, gl::usage::static_draw, tex_coords);
    m_normals.set(gl::target::array_buffer, gl::usage::static_draw, normals);
    m_indices.set(gl::target::element_array_buffer, gl::usage::static_draw, indices);
}

void mesh::color(const gl::color& color) {
    m_color = color;
}

void mesh::render(shader& shader) const {
    shader.set_uniform("color", m_color.data());
    shader.set_uniform("useColor", 1);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    m_positions.map_to_shader(gl::target::array_buffer, 0, 3);
    m_tex_coords.map_to_shader(gl::target::array_buffer, 1, 2);
    m_normals.map_to_shader(gl::target::array_buffer, 2, 3);
    m_indices.draw(gl::draw_mode::triangles, m_draw_count);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

mesh cube_mesh(const float length, const float width, const float height) {
    const auto hx = length / 2.0f;
    const auto hy = width / 2.0f;
    const auto hz = height / 2.0f;

    const float positions[] = {
        // Front face (z = hz) - Looking from +Z
        -hx, -hy,  hz,   hx, -hy,  hz,   hx,  hy,  hz,  -hx,  hy,  hz,
        // Back face (z = -hz) - Looking from -Z
         hx, -hy, -hz,  -hx, -hy, -hz,  -hx,  hy, -hz,   hx,  hy, -hz,
        // Top face (y = hy) - Looking from +Y
        -hx,  hy,  hz,   hx,  hy,  hz,   hx,  hy, -hz,  -hx,  hy, -hz,
        // Bottom face (y = -hy) - Looking from -Y
        -hx, -hy, -hz,   hx, -hy, -hz,   hx, -hy,  hz,  -hx, -hy,  hz,
        // Right face (x = hx) - Looking from +X
         hx, -hy,  hz,   hx, -hy, -hz,   hx,  hy, -hz,   hx,  hy,  hz,
        // Left face (x = -hx) - Looking from -X
        -hx, -hy, -hz,  -hx, -hy,  hz,  -hx,  hy,  hz,  -hx,  hy, -hz
    };

    constexpr float tex_coords[] = {
        0,0, 1,0, 1,1, 0,1, // Front
        0,0, 1,0, 1,1, 0,1, // Back
        0,0, 1,0, 1,1, 0,1, // Top
        0,0, 1,0, 1,1, 0,1, // Bottom
        0,0, 1,0, 1,1, 0,1, // Right
        0,0, 1,0, 1,1, 0,1  // Left
    };

    constexpr float normals[] = {
        0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,  // Front
        0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1,  // Back
        0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,  // Top
        0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,  // Bottom
        1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,  // Right
       -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0   // Left
    };

    unsigned int indices[36];
    for (int i = 0; i < 6; i++) {
        const auto v = i * 4;
        indices[i * 6 + 0] = v;     indices[i * 6 + 1] = v + 1; indices[i * 6 + 2] = v + 2;
        indices[i * 6 + 3] = v + 2; indices[i * 6 + 4] = v + 3; indices[i * 6 + 5] = v;
    }

    return mesh{positions, tex_coords, normals, indices};
}

}
