#pragma once

#include "../resource/propertiesResource.h"
#include "bulletPhysicsProvider.h"
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
    explicit bulletColliderResource(const std::string& identifier_) : propertiesResource(identifier_) {}
    void compile(const nlohmann::json& properties) override {
        this->mass = properties["properties"]["mass"];

        switch (bulletColliderResource::getBulletColliderTypeFromString(properties["properties"]["colliderType"])) {
            case BULLET_BOX:
                float boundX = properties["properties"]["boundX"];
                float boundY = properties["properties"]["boundY"];
                float boundZ = properties["properties"]["boundZ"];

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

                //add the body to the dynamics world
                //dynamicsWorld->addRigidBody(body);
                break;
        }
    }
    btRigidBody* getNewRigidBodyFromCollider() {
        //auto* myMotionState = new btDefaultMotionState(bodyTransform);
        //return new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(this->mass, myMotionState, this->collider.get(), localInertia));
        return nullptr; // todo
    }
    static bulletColliderType getBulletColliderTypeFromString(const std::string& colliderTypeStr) {
        // todo: add all collider types
        return BULLET_BOX;
    }
private:
    std::unique_ptr<btCollisionShape> collider;
    btScalar mass = 0.0;
};
