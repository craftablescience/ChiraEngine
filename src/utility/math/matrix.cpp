#include "matrix.h"

glm::mat4 chira::transformToMatrix(const glm::mat4& startMatrix, const glm::vec3& position, const glm::quat& rotation) {
    return glm::translate(startMatrix, position) * glm::mat4_cast(rotation);
}

glm::mat4 chira::transformToMatrixScaled(const glm::mat4& startMatrix, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {
    return glm::scale(transformToMatrix(startMatrix, position, rotation), scale);
}
