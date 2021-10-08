#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <btBulletDynamicsCommon.h>

namespace chira {
    glm::vec3 bulletToGLM(const btVector3& vector3);
    glm::quat bulletToGLM(const btQuaternion& quaternion);
    btVector3 glmToBullet(const glm::vec3& vector3);
}
