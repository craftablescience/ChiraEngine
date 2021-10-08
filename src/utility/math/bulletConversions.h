#pragma once

namespace chira {
    glm::vec3 bulletToGLM(const btVector3& vector3) {
        return {vector3.x(), vector3.y(), vector3.z()};
    }
    glm::quat bulletToGLM(const btQuaternion& quaternion) {
        return {quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z()};
    }
}
