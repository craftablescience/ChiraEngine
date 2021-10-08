#include "propBulletPhysicsEntity.h"

#include "../utility/math/bulletConversions.h"

using namespace chira;

propBulletPhysicsEntity* propBulletPhysicsEntity::init(meshComponent* mesh_, bulletRigidBodyComponent* physics_) {
    this->meshPtr = mesh_;
    this->add(mesh_);
    this->physics = physics_;
    this->add(physics_);
    return this;
}

void propBulletPhysicsEntity::preRender(double delta) {
    this->meshPtr->setPosition(bulletToGLM(this->physics->getTransform().getOrigin()));
    this->meshPtr->setRotation(bulletToGLM(this->physics->getTransform().getRotation()));
}
