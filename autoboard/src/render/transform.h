#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ui::render {

struct transform {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

inline glm::mat4 transformation(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * glm::mat4_cast(rotation);
    model = glm::scale(model, scale);
    return model;
}

}
