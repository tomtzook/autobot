#pragma once

#include <string>
#include <map>

#include "render/gl.h"

namespace ui::render {

class shader {
public:
    shader(std::string_view vertex_data, std::string_view fragment_data);

    void bind();
    void unbind();

    template<typename t_>
    void set_uniform(const std::string_view name, const t_ data) {
        auto& uniform = get_uniform(name);
        uniform.set(data);
    }

private:
    gl::uniform& get_uniform(const std::string_view& name);

    gl::shader m_vertex_shader;
    gl::shader m_fragment_shader;
    gl::program m_program;

    std::map<std::string, gl::uniform, std::less<>> m_uniforms;
};

}
