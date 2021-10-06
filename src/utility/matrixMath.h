#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace chira {
    glm::mat4 transformToMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale = glm::vec3{1}) {
        auto matrix = glm::identity<glm::mat4>();
        matrix = glm::translate(matrix, position);
        matrix *= glm::yawPitchRoll(rotation.x, rotation.y, rotation.z);
        matrix = glm::scale(matrix, scale);
        return matrix;
    }
}
