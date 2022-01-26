#pragma once

#include <memory>
#include <glm/vec3.hpp>
#include <entity/physics/abstractRigidBody.h>

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

        /// Engine::preInit should be called to use this!\n
        /// This should be set when running custom init callbacks.\n
        /// If this is not Bullet, all Bullet entities will crash the engine.
        [[nodiscard]] static AbstractPhysicsProvider* getPhysicsProvider();
        static void setPhysicsProvider(AbstractPhysicsProvider* newPhysicsProvider);
    private:
        static std::unique_ptr<AbstractPhysicsProvider> physicsProvider;
    };
}
