#pragma once

#include <map>
#include <string>

#include "data/source.h"
#include "render/transform.h"
#include "render/render.h"

namespace ui::render {

struct ligament_base {
    std::optional<mesh> mesh;
    glm::vec3 offset;
    glm::quat rotation;

    void render(const transform& root, renderer::context& renderer) const;
};

class ligament_box;
using ligament_type = std::optional<std::variant<ligament_box>>;

class ligament_box {
public:
    explicit ligament_box(data::data_source source);

    void update();
    void render(const transform& root, renderer::context& renderer) const;

private:
    data::data_source m_source;

    ligament_base m_base;
    float m_length;
    float m_width;
    float m_height;
};

class ligament_wrapper {
public:
    explicit ligament_wrapper(data::data_source source);

    void update();
    void render(const transform& root, renderer::context& renderer) const;

private:
    data::data_source m_source;
    ligament_type m_ligament;
};

class body {
public:
    explicit body(data::data_source source);

    void update();
    void render(renderer::context& renderer) const;

private:
    data::data_source m_source;
    transform m_transform;
    std::unordered_map<uint64_t, ligament_wrapper> m_ligaments;
};

class world3d {
public:
    world3d() = default;

    void attach_data(data::data_source&& source);
    void update();
    void render(renderer::context& renderer) const;

private:
    std::unordered_map<uint64_t, body> m_bodies;
};

}
