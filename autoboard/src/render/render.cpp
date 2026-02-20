
#include "render.h"

namespace ui::render {

static constexpr auto object_shader_vertex = R"(
    #version 330

	layout (location =0) in vec3 position;
	layout (location =1) in vec2 textCoord;

	out vec2 outTextCoord;

	uniform mat4 projection;
	uniform mat4 transformation;

	void main() {
		gl_Position = projection * transformation * vec4(position, 1);
		outTextCoord = textCoord;
	}
)";

static constexpr auto object_shader_fragment = R"(
    #version 330

	in vec2 outTextCoord;
	out vec4 fragColor;

	uniform sampler2D textureSampler;
	uniform vec4 color;
	uniform int useColor;

	void main() {
	    if(useColor == 1) {
			fragColor = color;
		} else {
			fragColor = texture2D(textureSampler, outTextCoord);
		}
	}
)";

renderer::context::context(shader& shader, const glm::mat4& view)
	: m_shader(shader)
	, m_view(view)
{}

renderer::context::~context() {
	m_shader.unbind();
}

void renderer::context::render(const glm::mat4& transform, const mesh& mesh) {
	const auto viewed_transform = m_view * transform;
	m_shader.set_uniform("transformation", viewed_transform);

	mesh.render(m_shader);
}

renderer::renderer()
	: m_shader(object_shader_vertex, object_shader_fragment)
{}

renderer::context renderer::start(const glm::mat4& projection, const glm::mat4& view) {
	m_shader.bind();
	m_shader.set_uniform("projection", projection);
	return context{m_shader, view};
}

}
