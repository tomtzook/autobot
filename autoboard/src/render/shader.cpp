
#include "shader.h"

#include <stdexcept>

namespace ui::render {

shader::shader(const std::string_view vertex_data, const std::string_view fragment_data)
    : m_vertex_shader(gl::shader_type::vertex)
    , m_fragment_shader(gl::shader_type::fragment)
    , m_program() {
    m_vertex_shader.source(vertex_data);
    m_vertex_shader.compile();
    if (const auto result = m_vertex_shader.get_param(gl::shader_param::compile_status); result != 1) {
        throw std::runtime_error("failed to compile vertex shader");
    }

    m_fragment_shader.source(fragment_data);
    m_fragment_shader.compile();
    if (const auto result = m_fragment_shader.get_param(gl::shader_param::compile_status); result != 1) {
        throw std::runtime_error("failed to compile fragment shader");
    }

    m_program.attach(m_vertex_shader);
    m_program.attach(m_fragment_shader);

    m_program.link();
    if (const auto result = m_program.get_param(gl::program_param::link_status); result != 1) {
        throw std::runtime_error("failed to link program");
    }

    m_program.validate();
    if (const auto result = m_program.get_param(gl::program_param::validate_status); result != 1) {
        throw std::runtime_error("failed to validate program");
    }
}

void shader::bind() {
    m_program.use();
}

void shader::unbind() {
    m_program.unuse();
}

gl::uniform& shader::get_uniform(const std::string_view& name) {
    if (const auto it = m_uniforms.find(name); it != m_uniforms.end()) {
        return it->second;
    }

    auto uniform = m_program.find_uniform(name);
    auto [it, _] = m_uniforms.emplace(name, uniform);

    return it->second;
}

}
