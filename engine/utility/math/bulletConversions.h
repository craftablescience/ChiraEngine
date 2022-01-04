#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <btBulletDynamicsCommon.h>

namespace chira {
    inline glm::vec3 bulletToGLM(const btVector3 vector3) {
        return {vector3.x(), vector3.y(), vector3.z()};
    }
    inline glm::quat bulletToGLM(const btQuaternion quaternion) {
        return {quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z()};
    }
    inline btVector3 glmToBullet(const glm::vec3 vector3) {
        return {vector3.x, vector3.y, vector3.z};
    }
    inline btQuaternion glmToBullet(const glm::quat quaternion) {
        return {quaternion.x, quaternion.y, quaternion.z, quaternion.w};
    }
}
