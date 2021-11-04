#pragma once

#include "bulletColliderResource.h"
#include "../utility/math/bulletConversions.h"
#include <string>

namespace chira {
    /// This class assumes Bullet is the current physics provider.
    /// Things will break if it is not.
    class bulletRigidBodyComponent {
    public:
        explicit bulletRigidBodyComponent(const std::string& identifier, const glm::vec3& transform = glm::vec3{});
        ~bulletRigidBodyComponent();
        void render(double delta) {}
        const btTransform& getTransform() {
            return this->rigidBody->getWorldTransform();
        }
    private:
        bulletColliderResource* collider;
        btRigidBody* rigidBody;
    };
}
