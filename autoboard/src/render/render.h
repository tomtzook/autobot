#pragma once

#include "render/gl.h"
#include "render/shader.h"
#include "render/mesh.h"

namespace ui::render {

class renderer {
public:
    class context {
    public:
        explicit context(shader& shader, const glm::mat4& view);
        ~context();

        void render(const glm::mat4& transform, const mesh& mesh);

    private:
        shader& m_shader;
        const glm::mat4& m_view;
    };

    renderer();

    context start(const glm::mat4& projection, const glm::mat4& view);

private:
    shader m_shader;
};

}
