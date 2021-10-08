#pragma once

#include "../wec/abstractComponent.h"
#include "bulletColliderResource.h"
#include "../utility/math/bulletConversions.h"
#include <string>

namespace chira {
    /// This class assumes Bullet is the current physics provider.
    /// Things will break if it is not.
    class bulletRigidBodyComponent : public abstractComponent {
    public:
        explicit bulletRigidBodyComponent(const std::string& identifier, const glm::vec3& transform = glm::vec3{});
        ~bulletRigidBodyComponent() override;
        void render(double delta) override {}
        const btTransform& getTransform() {
            return this->rigidBody->getWorldTransform();
        }
    private:
        bulletColliderResource* collider;
        btRigidBody* rigidBody;
    };
}
