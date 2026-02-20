
#include "camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace ui::render {

camera::camera(const glm::mat4& projection, const float movement_speed, const float look_sensitivity)
    : m_projection(projection)
    , m_movement_speed(movement_speed)
    , m_look_sensitivity(look_sensitivity)
    , m_position()
    , m_rotation(1, 0, 0, 0)
{}

camera::camera(const float fov, const float aspect_ratio, const float z_near, const float z_far,
    const float movement_speed, const float look_sensitivity)
    : camera(glm::perspective(fov, aspect_ratio, z_near, z_far), movement_speed, look_sensitivity)
{}

const glm::mat4& camera::projection() const {
    return m_projection;
}

glm::mat4 camera::view() const {
    const auto rotation = glm::mat4_cast(glm::conjugate(m_rotation));
    const auto translation = glm::translate(glm::mat4(1.0f), -m_position);
    return rotation * translation;
}

const glm::vec3& camera::position() const {
    return m_position;
}

const glm::quat& camera::rotation() const {
    return m_rotation;
}

void camera::set_position(const glm::vec3& position) {
    m_position = position;
}

void camera::set_rotation(const glm::quat& rotation) {
    m_rotation = rotation;
}

void camera::rotate_yaw(const float angle_rad) {
    constexpr auto up_axis = glm::vec3(0.0f, 1.0f, 0.0f);
    const auto delta_rotation = glm::angleAxis(angle_rad, up_axis);
    m_rotation = glm::normalize(m_rotation * delta_rotation);
}

}
