#include "bulletRigidBody.h"

#include <core/engine.h>
#include <physics/bulletPhysicsProvider.h>
#include <resource/resource.h>
#include <utility/math/bulletConversions.h>

using namespace chira;

bulletRigidBody::bulletRigidBody(entity* parent_, const std::string& colliderId) : entity3d(parent_) {
    this->collider = resource::getResource<bulletColliderResource>(colliderId);
    this->rigidBody = this->collider->getNewRigidBody();
    // todo: this only accounts for position, not rotation
    this->rigidBody->translate(glmToBullet(this->position));
    dynamic_cast<bulletPhysicsProvider*>(engine::getPhysicsProvider())->addRigidBody(this->rigidBody);
}

bulletRigidBody::bulletRigidBody(entity* parent_, const std::string& name_, const std::string& colliderId) : entity3d(parent_, name_) {
    this->collider = resource::getResource<bulletColliderResource>(colliderId);
    this->rigidBody = this->collider->getNewRigidBody();
    // todo: this only accounts for position, not rotation
    this->rigidBody->translate(glmToBullet(this->position));
    dynamic_cast<bulletPhysicsProvider*>(engine::getPhysicsProvider())->addRigidBody(this->rigidBody);
}

bulletRigidBody::bulletRigidBody(const std::string& colliderId) : entity3d() {
    this->collider = resource::getResource<bulletColliderResource>(colliderId);
    this->rigidBody = this->collider->getNewRigidBody();
    // todo: this only accounts for position, not rotation
    this->rigidBody->translate(glmToBullet(this->position));
    dynamic_cast<bulletPhysicsProvider*>(engine::getPhysicsProvider())->addRigidBody(this->rigidBody);
}

bulletRigidBody::bulletRigidBody(const std::string& name_, const std::string& colliderId) : entity3d(nullptr, name_) {
    this->collider = resource::getResource<bulletColliderResource>(colliderId);
    this->rigidBody = this->collider->getNewRigidBody();
    // todo: this only accounts for position, not rotation
    this->rigidBody->translate(glmToBullet(this->position));
    dynamic_cast<bulletPhysicsProvider*>(engine::getPhysicsProvider())->addRigidBody(this->rigidBody);
}

bulletRigidBody::~bulletRigidBody() {
    dynamic_cast<bulletPhysicsProvider*>(engine::getPhysicsProvider())->removeRigidBody(this->rigidBody);
}

void bulletRigidBody::render(const glm::mat4& parentTransform) {
    this->position = bulletToGLM(this->rigidBody->getWorldTransform().getOrigin());
    this->rotation = bulletToGLM(this->rigidBody->getWorldTransform().getRotation());
    entity3d::render(parentTransform);
}

void bulletRigidBody::setPosition(const glm::vec3& newPos) {
    // todo: set position of rigidbody
    entity3d::setPosition(newPos);
}

void bulletRigidBody::setRotation(const glm::quat& newRot) {
    // todo: set rotation of rigidbody
    entity3d::setRotation(newRot);
}

const glm::vec3& bulletRigidBody::getPosition() {
    this->position = bulletToGLM(this->rigidBody->getWorldTransform().getOrigin());
    return entity3d::getPosition();
}

const glm::quat& bulletRigidBody::getRotation() {
    this->rotation = bulletToGLM(this->rigidBody->getWorldTransform().getRotation());
    return entity3d::getRotation();
}

void bulletRigidBody::translate(const glm::vec3& translateByAmount) {
    this->rigidBody->translate(glmToBullet(translateByAmount));
    entity3d::translate(translateByAmount);
}

void bulletRigidBody::rotate(const glm::quat& rotateByAmount) {
    // todo: rotate rigidbody
    entity3d::rotate(rotateByAmount);
}
