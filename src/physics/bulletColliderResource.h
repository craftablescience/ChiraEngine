#pragma once

#include "../resource/propertiesResource.h"
#include <btBulletDynamicsCommon.h>

namespace chira {
    enum bulletColliderType {
        BULLET_BOX
    };

    /// Note: this does not add or remove the collider to the physics world.
    /// This must be done by a component.
    class bulletColliderResource : public propertiesResource {
    public:
        explicit bulletColliderResource(const std::string& identifier_) : propertiesResource(identifier_) {}
        void compile(const nlohmann::json& properties) override {
            this->mass = properties["properties"]["mass"];
            if (properties["properties"].contains("originX")) {
                this->originX = properties["properties"]["originX"];
            }
            if (properties["properties"].contains("originY")) {
                this->originY = properties["properties"]["originY"];
            }
            if (properties["properties"].contains("originZ")) {
                this->originZ = properties["properties"]["originZ"];
            }
            switch (bulletColliderResource::getBulletColliderTypeFromString(properties["properties"]["colliderType"])) {
                case BULLET_BOX:
                    float boundX = properties["properties"]["boundX"];
                    float boundY = properties["properties"]["boundY"];
                    float boundZ = properties["properties"]["boundZ"];
                    this->collider = std::make_unique<btBoxShape>(btVector3(boundX, boundY, boundZ));

                    this->transform.setIdentity();
                    this->transform.setOrigin(btVector3(this->originX, this->originY, this->originZ));

                    bool isDynamic = this->mass != 0.0f;
                    if (isDynamic) {
                        this->collider->calculateLocalInertia(this->mass, this->localInertia);
                    }
                    break;
            }
        }
        btRigidBody* getNewRigidBody() {
            btRigidBody::btRigidBodyConstructionInfo info(
                    this->mass,
                    new btDefaultMotionState(this->transform),
                    this->collider.get(),
                    this->localInertia);
            return new btRigidBody(info);
        }
        static bulletColliderType getBulletColliderTypeFromString(const std::string& colliderTypeStr) {
            // todo: add all collider types
            return BULLET_BOX;
        }
    private:
        std::unique_ptr<btCollisionShape> collider;
        btScalar mass = 0.0;
        btTransform transform;
        btVector3 localInertia;
        btScalar originX = 0.0;
        btScalar originY = 0.0;
        btScalar originZ = 0.0;
    };
}
