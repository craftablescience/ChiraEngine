#include "matrix.h"

glm::mat4 chira::transformToMatrix(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {
    auto matrix = glm::translate(glm::identity<glm::mat4>(), position);
    matrix = matrix * glm::mat4_cast(rotation);
    matrix = glm::scale(matrix, scale);
    return matrix;
}
