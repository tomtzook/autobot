
#include "body.h"

#include <ranges>

#include "data/registry.h"

namespace ui::render {
static ligament_type create_ligament_by_type(const data::data_source& source) {
    switch (source.get_scheme()) {
        case data::scheme::type::d3_box:
            return ligament_box(source);
        default:
            return std::nullopt;
    }
}

static bool should_reload_ligament(const ligament_type& current_ligament, const data::data_source& source) {
    switch (source.get_scheme()) {
        case data::scheme::type::d3_box:
            return !current_ligament.has_value() || !std::holds_alternative<ligament_box>(current_ligament.value());
        default:
            return false;
    }
}

void ligament_base::render(const transform& root, renderer::context& renderer) const {
    if (!mesh) {
        return;
    }

    const auto root_mat = transformation(root.position, root.rotation, glm::vec3(1.0f));
    const auto off_mat = transformation(offset, rotation, glm::vec3(1.0f));
    const auto abs_mat = root_mat * off_mat;

    renderer.render(abs_mat, mesh.value());
}

ligament_box::ligament_box(data::data_source source)
    : m_source(std::move(source))
    , m_base()
    , m_length()
    , m_width()
    , m_height()
{}

void ligament_box::update() {
    if (m_source.get_scheme() != data::scheme::type::d3_box || !m_source.has_new_data()) {
        return;
    }

    const auto data = m_source.read<data::scheme::d3_box>();
    m_base.offset.x = data.x;
    m_base.offset.y = data.y;
    m_base.offset.z = data.z;
    m_base.rotation = glm::vec3(data.pitch, data.yaw, data.roll);

    if (m_length != data.length || m_width != data.width || m_height != data.height) {
        m_length = data.length;
        m_width = data.width;
        m_height = data.height;
        m_base.mesh = cube_mesh(m_length, m_width, m_height); // todo: color
        m_base.mesh->color(gl::color(255, 0, 0, 255));
    }
}

void ligament_box::render(const transform& root, renderer::context& renderer) const {
    m_base.render(root, renderer);
}

ligament_wrapper::ligament_wrapper(data::data_source source)
    : m_source(std::move(source))
    , m_ligament()
{}

void ligament_wrapper::update() {
    if (should_reload_ligament(m_ligament, m_source)) {
        if (auto new_ligament = create_ligament_by_type(m_source)) {
            m_ligament.emplace(std::move(new_ligament.value()));
        } else {
            m_ligament.reset();
        }
    }

    if (!m_ligament) {
        return;
    }

    auto& shape = m_ligament.value();
    std::visit([](auto& value) {
        value.update();
    }, shape);
}

void ligament_wrapper::render(const transform& root, renderer::context& renderer) const {
    if (!m_ligament) {
        return;
    }

    auto& ligament = m_ligament.value();
    std::visit([&root, &renderer](auto& value) {
        value.render(root, renderer);
    }, ligament);
}

body::body(data::data_source source)
    : m_source(std::move(source))
    , m_transform()
    , m_ligaments()
{}

void body::update() {
    if (m_source.get_scheme() != data::scheme::type::d3_body || !m_source.has_new_data()) {
        return;
    }

    if (m_source.has_new_data_self()) {
        const auto data = m_source.read<data::scheme::d3_body>();
        m_transform.position.x = data.x;
        m_transform.position.y = data.y;
        m_transform.position.z = data.z;
        m_transform.rotation = glm::vec3(data.pitch, data.yaw, data.roll);
        m_transform.scale = glm::vec3(1.0f);

        auto& registry = data::get_registry();
        registry.foreach_child(m_source.get_id(), [this](auto& source)->void {
            if (const auto it = m_ligaments.find(source.get_id()); it != m_ligaments.end()) {
                return;
            }
            if (!source.has_children()) {
                return;
            }

            m_ligaments.emplace(source.get_id(), std::move(source));
        });
    }

    for (auto& ligament : m_ligaments | std::views::values) {
        ligament.update();
    }
}

void body::render(renderer::context& renderer) const {
    for (const auto& ligament : m_ligaments | std::views::values) {
        ligament.render(m_transform, renderer);
    }
}

void world3d::attach_data(data::data_source&& source) {
    m_bodies.emplace(source.get_id(), std::move(source));
}

void world3d::update() {
    for (auto& body: m_bodies | std::views::values) {
        body.update();
    }

    auto& registry = data::get_registry();
    registry.foreach([this](auto& source) {
        if (source.get_scheme() == data::scheme::type::d3_body) {
            if (const auto it = m_bodies.find(source.get_id()); it == m_bodies.end()) {
                m_bodies.emplace(source.get_id(), source);
            }
        }
    });
}

void world3d::render(renderer::context& renderer) const {
    for (const auto& body: m_bodies | std::views::values) {
        body.render(renderer);
    }
}

}
