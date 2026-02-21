#pragma once

#include <glm/gtc/quaternion.hpp>

#include "render/gl.h"

namespace ui::render {

class camera {
public:
    explicit camera(const glm::mat4& projection);
    camera(float fov, float aspect_ratio, float z_near, float z_far);

    [[nodiscard]] const glm::mat4& projection() const;
    [[nodiscard]] glm::mat4 view() const;

    [[nodiscard]] const glm::vec3& position() const;
    [[nodiscard]] const glm::quat& rotation() const;
    void set_position(const glm::vec3& position);
    void set_rotation(const glm::quat& rotation);

    void move_forward(float distance);
    void move_backward(float distance);
    void move_left(float distance);
    void move_right(float distance);
    void rotate_yaw(float angle_rad);
    void rotate_pitch(float angle_rad);

private:
    const glm::mat4 m_projection;

    glm::vec3 m_position;
    glm::quat m_rotation;
};

}
