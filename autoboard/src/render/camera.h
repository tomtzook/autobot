#pragma once

#include <glm/gtc/quaternion.hpp>

#include "render/gl.h"

namespace ui::render {

class camera {
public:
    camera(const glm::mat4& projection, float movement_speed, float look_sensitivity);
    camera(float fov, float aspect_ratio, float z_near, float z_far, float movement_speed, float look_sensitivity);

    [[nodiscard]] const glm::mat4& projection() const;
    [[nodiscard]] glm::mat4 view() const;

    [[nodiscard]] const glm::vec3& position() const;
    [[nodiscard]] const glm::quat& rotation() const;
    void set_position(const glm::vec3& position);
    void set_rotation(const glm::quat& rotation);

    void rotate_yaw(float angle_rad);

private:
    const glm::mat4 m_projection;
    const float m_movement_speed;
    const float m_look_sensitivity;

    glm::vec3 m_position;
    glm::quat m_rotation;
};

}
