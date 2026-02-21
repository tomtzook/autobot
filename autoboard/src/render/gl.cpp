
#include <glm/gtc/type_ptr.hpp>

#include "gl.h"

namespace gl {

color::color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
    : m_data{} {
    set(red, green, blue, alpha);
}

std::span<const float> color::data() const {
    return m_data;
}

void color::set(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha) {
    m_data[0] = static_cast<float>(red) / 255.0f;
    m_data[1] = static_cast<float>(green) / 255.0f;
    m_data[2] = static_cast<float>(blue) / 255.0f;
    m_data[3] = static_cast<float>(alpha) / 255.0f;
}

shader::shader(const shader_type type)
    : m_id(glCreateShader(static_cast<GLenum>(type)))
{}

shader::~shader() {
    if (m_id != 0) {
        glDeleteShader(m_id);
        m_id = 0;
    }
}

shader::shader(shader&& other) noexcept
    : m_id(other.m_id) {
    other.m_id = 0;
}

shader& shader::operator=(shader&& other) noexcept {
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

int32_t shader::get_param(const shader_param param) const {
    GLint value = 0;
    glGetShaderiv(m_id, static_cast<GLenum>(param), &value);
    return value;
}

void shader::source(const std::string_view data) {
    const char* source = data.data();
    glShaderSource(m_id, 1, &source, nullptr);
}

void shader::compile() {
    glCompileShader(m_id);
    // todo: check and throw
}

uniform::uniform(const GLint location)
    : m_location(location)
{}

void uniform::set(const int i) {
    glUniform1i(m_location, i);
}

void uniform::set(const std::span<const float> data) {
    switch (data.size()) {
        case 1:
            glUniform1fv(m_location, 1, data.data());
            break;
        case 2:
            glUniform2fv(m_location, 1, data.data());
            break;
        case 3:
            glUniform3fv(m_location, 1, data.data());
            break;
        case 4:
            glUniform4fv(m_location, 1, data.data());
            break;
        default:
            break;
    }
}

void uniform::set(const glm::mat4& matrix) {
    glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

program::program()
    : m_id(glCreateProgram())
{}

program::~program() {
    if (m_id != 0) {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}

program::program(program&& other) noexcept
    : m_id(other.m_id) {
    other.m_id = 0;
}

program& program::operator=(program&& other) noexcept {
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

int32_t program::get_param(const program_param param) const {
    GLint value = 0;
    glGetProgramiv(m_id, static_cast<GLenum>(param), &value);
    return value;
}

uniform program::find_uniform(const std::string_view name) const {
    const auto location = glGetUniformLocation(m_id, name.data());
    if (location == -1) {
        // todo: throw
    }

    return uniform{location};
}

void program::attach(const shader& shader) {
    glAttachShader(m_id, shader.m_id);
}

void program::detach(const shader& shader) {
    glDetachShader(m_id, shader.m_id);
}

void program::link() {
    glLinkProgram(m_id);
    // todo: check and throw
}

void program::validate() {
    glValidateProgram(m_id);
    // todo: check and throw
}

void program::use() {
    glUseProgram(m_id);
}

void program::unuse() {
    glUseProgram(0);
}

}
