#pragma once

#include <span>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace gl {

enum class target {
    array_buffer = GL_ARRAY_BUFFER,
    element_array_buffer = GL_ELEMENT_ARRAY_BUFFER,
};

enum class usage {
    static_draw = GL_STATIC_DRAW,
};

enum class draw_mode {
    triangles = GL_TRIANGLES,
};

enum class shader_type {
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER,
};

enum class shader_param {
    compile_status = GL_COMPILE_STATUS
};

enum class program_param {
    link_status = GL_LINK_STATUS,
    validate_status = GL_VALIDATE_STATUS,
};

template<typename t_>
struct gl_type {};
template<>
struct gl_type<unsigned int> { static constexpr auto type = GL_UNSIGNED_INT; };
template<>
struct gl_type<float> { static constexpr auto type = GL_FLOAT; };

template<typename t_>
struct buffer {
    buffer();
    ~buffer();

    void set(target target, usage usage, std::span<const t_> data);

    void map_to_shader(target target, size_t index, size_t size) const;
    void draw(draw_mode mode, size_t count) const;

private:
    GLuint m_id;
};

using uint_buffer = buffer<unsigned int>;
using float_buffer = buffer<float>;

struct color {
    explicit color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255);

    [[nodiscard]] std::span<const float> data() const;
    void set(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255);

private:
    float m_data[4];
};

struct shader {
    explicit shader(shader_type type);
    ~shader();

    [[nodiscard]] int32_t get_param(shader_param param) const;

    void source(std::string_view data);
    void compile();

private:
    GLuint m_id;

    friend struct program;
};

struct uniform {
    explicit uniform(GLint location);

    void set(int i);
    void set(std::span<const float> data);
    void set(const glm::mat4& matrix);

private:
    GLint m_location;
};

struct program {
    program();
    ~program();

    [[nodiscard]] int32_t get_param(program_param param) const;
    [[nodiscard]] uniform find_uniform(std::string_view name) const;

    void attach(const shader& shader);
    void detach(const shader& shader);

    void link();
    void validate();
    void use();
    void unuse();

private:
    GLuint m_id;
};

template<typename t_>
buffer<t_>::buffer()
    : m_id(0) {
    glGenBuffers(1, &m_id);
}

template<typename t_>
buffer<t_>::~buffer() {
    glDeleteBuffers(1, &m_id);
}

template<typename t_>
void buffer<t_>::set(const target target, const usage usage, const std::span<const t_> data) {
    glBindBuffer(static_cast<GLenum>(target), m_id);
    glBufferData(static_cast<GLenum>(target), static_cast<GLsizeiptr>(data.size()), data.data(), static_cast<GLenum>(usage));
    glBindBuffer(static_cast<GLenum>(target), 0);
}

template<typename t_>
void buffer<t_>::map_to_shader(const target target, const size_t index, const size_t size) const {
    glBindBuffer(static_cast<GLenum>(target), m_id);
    glVertexAttribPointer(index, size, gl_type<t_>::type, false, 0, nullptr);
    glBindBuffer(static_cast<GLenum>(target), 0); // todo: maybe isn;t always needed
}

template<typename t_>
void buffer<t_>::draw(const draw_mode mode, const size_t count) const {
    glBindBuffer(static_cast<GLenum>(target::element_array_buffer), m_id);
    glDrawElements(static_cast<GLenum>(mode), count, gl_type<t_>::type, nullptr);
    glBindBuffer(static_cast<GLenum>(target::element_array_buffer), 0); // todo: maybe isn;t always needed
}

}
