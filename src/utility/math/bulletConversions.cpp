#include "bulletConversions.h"

glm::vec3 chira::bulletToGLM(const btVector3& vector3) {
    return {vector3.x(), vector3.y(), vector3.z()};
}

glm::quat chira::bulletToGLM(const btQuaternion& quaternion) {
    return {quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z()};
}

btVector3 chira::glmToBullet(const glm::vec3& vector3) {
    return {vector3.x, vector3.y, vector3.z};
}
