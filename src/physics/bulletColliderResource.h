#pragma once

#include "../resource/propertiesResource.h"
#include "bulletPhysicsWorld.h"
#include <btBulletDynamicsCommon.h>

enum bulletColliderType {
    BULLET_BOX
};

/*
 * Note that this does not add or remove the collider to the physics world.
 * This must be done by a component.
 */
class bulletColliderResource : propertiesResource {
public:
    bulletColliderResource(const std::string& provider_, const std::string& name_) : propertiesResource(provider_, name_) {}
    void compile(const nlohmann::json& properties) override {
        switch (bulletColliderResource::getBulletColliderTypeFromString(properties["properties"]["colliderType"])) {
            case BULLET_BOX:
                float boundX = properties["properties"]["boundX"];
                float boundY = properties["properties"]["boundY"];
                float boundZ = properties["properties"]["boundZ"];
                float mass = properties["properties"]["mass"];

                this->collider = std::make_unique<btBoxShape>(btVector3(boundX, boundY, boundZ));
                btTransform bodyTransform;
                bodyTransform.setIdentity();
                // todo: should this should be set by a component now or later?
                bodyTransform.setOrigin(btVector3(0, 0, 0));

                bool isDynamic = mass != 0.0f;
                btVector3 localInertia(0, 0, 0);
                if (isDynamic) {
                    this->collider->calculateLocalInertia(mass, localInertia);
                }

                auto* myMotionState = new btDefaultMotionState(bodyTransform);
                auto* body = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(mass, myMotionState, this->collider.get(), localInertia));

                //add the body to the dynamics world
                dynamicsWorld->addRigidBody(body);

                break;
        }
    }
    static bulletColliderType getBulletColliderTypeFromString(const std::string& colliderTypeStr) {
        // todo: add all collider types
        return BULLET_BOX;
    }
private:
    std::unique_ptr<btCollisionShape> collider;
};
