#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace chira {
    glm::mat4 transformToMatrix(const glm::mat4& startMatrix = glm::identity<glm::mat4>(), const glm::vec2& position = glm::vec2{}, const glm::quat& rotation = glm::identity<glm::quat>());
    glm::mat4 transformToMatrixScaled(const glm::mat4& startMatrix = glm::identity<glm::mat4>(), const glm::vec2& position = glm::vec2{}, const glm::quat& rotation = glm::identity<glm::quat>(), const glm::vec3& scale = glm::vec3{1});
    glm::mat4 transformToMatrix(const glm::mat4& startMatrix = glm::identity<glm::mat4>(), const glm::vec3& position = glm::vec3{}, const glm::quat& rotation = glm::identity<glm::quat>());
    glm::mat4 transformToMatrixScaled(const glm::mat4& startMatrix = glm::identity<glm::mat4>(), const glm::vec3& position = glm::vec3{}, const glm::quat& rotation = glm::identity<glm::quat>(), const glm::vec3& scale = glm::vec3{1});
}
