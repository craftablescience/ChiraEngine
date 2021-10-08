#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace chira {
    glm::mat4 transformToMatrix(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale = glm::vec3{1});
}
