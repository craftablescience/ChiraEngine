#pragma once

#include "../entity3d.h"
#include "../../../physics/bulletColliderResource.h"

namespace chira {
    class bulletRigidBody : public entity3d {
    public:
        bulletRigidBody(entity* parent_, const std::string& colliderId);
        bulletRigidBody(entity* parent_, const std::string& name_, const std::string& colliderId);
        explicit bulletRigidBody(const std::string& colliderId);
        bulletRigidBody(const std::string& name_, const std::string& colliderId);
        ~bulletRigidBody() override;
        void render(const glm::mat4& parentTransform) override;
        void setPosition(const glm::vec3& newPos) override;
        void setRotation(const glm::quat& newRot) override;
        const glm::vec3& getPosition() override;
        const glm::quat& getRotation() override;
        void translate(const glm::vec3& translateByAmount) override;
        void rotate(const glm::quat& rotateByAmount) override;
    protected:
        sharedPointer<bulletColliderResource> collider;
        btRigidBody* rigidBody;
    };
}
