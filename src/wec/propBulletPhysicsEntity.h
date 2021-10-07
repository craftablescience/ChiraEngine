#pragma once

#include "abstractEntity.h"
#include "meshComponent.h"
#include "componentFactory.h"
#include "../physics/bulletRigidBodyComponent.h"

namespace chira {
    class propBulletPhysicsEntity : public abstractEntity {
    public:
        propBulletPhysicsEntity* init(meshComponent* mesh_, bulletRigidBodyComponent* physics_) {
            this->meshPtr = mesh_;
            this->add(mesh_);
            this->physics = physics_;
            this->add(physics_);
            return this;
        }
        void preRender(double delta) override {
            this->meshPtr->setPosition(propBulletPhysicsEntity::btToGLM(this->physics->getTransform().getOrigin()));
            this->meshPtr->setRotation(propBulletPhysicsEntity::btQuatToEuler(this->physics->getTransform().getRotation()));
        }
        void postRender(double delta) override {}
    private:
        meshComponent* meshPtr = nullptr;
        bulletRigidBodyComponent* physics = nullptr;

        RegisterComponentName(propBulletPhysicsEntity);

        static glm::vec3 btToGLM(const btVector3& vector3) {
            return glm::vec3{vector3.x(), vector3.y(), vector3.z()};
        }
        static glm::vec3 btQuatToEuler(const btQuaternion& quat) {
            btScalar x, y, z;
            quat.getEulerZYX(z, y, x);
            return glm::vec3{x,y,z};
        }
    };
}
