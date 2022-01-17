#pragma once

#include <glm/vec3.hpp>
#include <entity/3d/physics/abstractRigidBody.h>

namespace chira {
    class AbstractPhysicsProvider {
    public:
        virtual ~AbstractPhysicsProvider() = default;
        virtual void updatePhysics(double delta) = 0;
        virtual void stop() = 0;
        virtual void setGravity(glm::vec3 gravity) = 0;
        /// Returns the first rigidbody entity hit by the ray, null if none.
        [[nodiscard]] virtual AbstractRigidBody* traceRay(glm::vec3 start, glm::vec3 end) const = 0;
        [[nodiscard]] virtual AbstractRigidBody* traceRay(glm::vec3 start, glm::vec3 direction, float magnitude) const = 0;
    };
}
