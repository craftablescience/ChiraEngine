#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <btBulletDynamicsCommon.h>

namespace chira {
    /// This is a struct instead of raw functions because of some idiotic linker error.
    /// What's even scarier is the error messages pointed to random spots in the nlohmann_json header!
    struct bulletConversions {
        static glm::vec3 bulletToGLM(const btVector3& vector3) {
            return {vector3.x(), vector3.y(), vector3.z()};
        }
        static glm::quat bulletToGLM(const btQuaternion& quaternion) {
            return {quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z()};
        }
        static btVector3 glmToBullet(const glm::vec3& vector3) {
            return {vector3.x, vector3.y, vector3.z};
        }
    };
}
