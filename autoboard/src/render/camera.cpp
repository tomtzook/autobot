
#include "camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace ui::render {

camera::camera(const glm::mat4& projection)
    : m_projection(projection)
    , m_position()
    , m_rotation(1, 0, 0, 0)
{}

camera::camera(const float fov, const float aspect_ratio, const float z_near, const float z_far)
    : camera(glm::perspective(fov, aspect_ratio, z_near, z_far))
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

void camera::move_forward(const float distance) {
    constexpr auto axis = glm::vec3(0.0f, 0.0f, -1.0f);
    const auto forward = m_rotation * axis;
    m_position += forward * distance;
}

void camera::move_backward(const float distance) {
    constexpr auto axis = glm::vec3(0.0f, 0.0f, 1.0f);
    const auto backward = m_rotation * axis;
    m_position += backward * distance;
}

void camera::move_left(const float distance) {
    constexpr auto axis = glm::vec3(-1.0f, 0.0f, 0.0f);
    const auto left = m_rotation * axis;
    m_position += left * distance;
}

void camera::move_right(const float distance) {
    constexpr auto axis = glm::vec3(1.0f, 0.0f, 0.0f);
    const auto right = m_rotation * axis;
    m_position += right * distance;
}

void camera::rotate_yaw(const float angle_rad) {
    constexpr auto axis = glm::vec3(0.0f, 1.0f, 0.0f);
    const auto delta_rotation = glm::angleAxis(angle_rad, axis);
    m_rotation = glm::normalize(m_rotation * delta_rotation);
}

void camera::rotate_pitch(const float angle_rad) {
    constexpr auto axis = glm::vec3(1.0f, 0.0f, 0.0f);
    const auto delta_rotation = glm::angleAxis(angle_rad, axis);
    m_rotation = glm::normalize(m_rotation * delta_rotation);
}

}
