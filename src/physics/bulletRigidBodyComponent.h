#pragma once

#include "../wec/abstractComponent.h"
#include "bulletColliderResource.h"
#include <string>
#include <glm/glm.hpp>

namespace chira {
    /// This class assumes Bullet is the current physics provider.
    /// Things will break if it is not.
    class bulletRigidBodyComponent : public abstractComponent {
    public:
        explicit bulletRigidBodyComponent(const std::string& identifier, const glm::vec3& transform = glm::vec3{});
        ~bulletRigidBodyComponent() override {
            this->collider->release();
        }
        void render(double delta) override {}
        const btTransform& getTransform() {
            return this->rigidBody->getWorldTransform();
        }
    private:
        bulletColliderResource* collider;
        btRigidBody* rigidBody;

        static btVector3 glmToBt(const glm::vec3& vector3) {
            return {vector3.x, vector3.y, vector3.z};
        }
    };
}
