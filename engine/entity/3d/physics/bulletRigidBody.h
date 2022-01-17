#pragma once

#include "abstractRigidBody.h"
#include <physics/bulletColliderResource.h>

namespace chira {
    class BulletRigidBody : public AbstractRigidBody {
    public:
        BulletRigidBody(Entity* parent_, const std::string& colliderId);
        BulletRigidBody(Entity* parent_, const std::string& name_, const std::string& colliderId);
        explicit BulletRigidBody(const std::string& colliderId);
        BulletRigidBody(const std::string& name_, const std::string& colliderId);
        ~BulletRigidBody() override;
        void render(const glm::mat4& parentTransform) override;
        void setPosition(glm::vec3 newPos) override;
        void setRotation(glm::quat newRot) override;
        glm::vec3 getPosition() override;
        glm::quat getRotation() override;
        void translate(glm::vec3 translateByAmount) override;
        void rotate(glm::quat rotateByAmount) override;
    protected:
        SharedPointer<BulletColliderResource> collider;
        btRigidBody* rigidBody;
    private:
        // I'm not bothering with this
        using Entity3d::translateWithRotation;
    };
}
