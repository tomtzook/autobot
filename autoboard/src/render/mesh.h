#pragma once

#include "render/gl.h"
#include "render/shader.h"

namespace ui::render {

class mesh {
public:
    mesh(std::span<const float> positions,
        std::span<const float> tex_coords,
        std::span<const float> normals,
        std::span<const unsigned int> indices);

    void color(const gl::color& color);

    void render(shader& shader) const;

private:
    gl::float_buffer m_positions;
    gl::float_buffer m_tex_coords;
    gl::float_buffer m_normals;
    gl::uint_buffer m_indices;
    size_t m_draw_count;
    gl::color m_color;
};

mesh cube_mesh(float length, float width, float height);
mesh sphere_mesh(float radius);

}
