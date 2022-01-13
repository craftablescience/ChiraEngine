#include "bulletRigidBody.h"

#include <core/engine.h>
#include <physics/bulletPhysicsProvider.h>
#include <resource/resource.h>
#include <utility/math/bulletConversions.h>
#include <utility/debug/assertions.h>

using namespace chira;

BulletRigidBody::BulletRigidBody(Entity* parent_, const std::string& colliderId) : Entity3d(parent_) {
    this->collider = Resource::getResource<BulletColliderResource>(colliderId);
    this->rigidBody = this->collider->getNewRigidBody();
    this->rigidBody->translate(glmToBullet(this->position));
    assert_cast<BulletPhysicsProvider*>(Engine::getPhysicsProvider())->addRigidBody(this->rigidBody);
}

BulletRigidBody::BulletRigidBody(Entity* parent_, const std::string& name_, const std::string& colliderId) : Entity3d(parent_, name_) {
    this->collider = Resource::getResource<BulletColliderResource>(colliderId);
    this->rigidBody = this->collider->getNewRigidBody();
    this->rigidBody->translate(glmToBullet(this->position));
    assert_cast<BulletPhysicsProvider*>(Engine::getPhysicsProvider())->addRigidBody(this->rigidBody);
}

BulletRigidBody::BulletRigidBody(const std::string& colliderId) : Entity3d() {
    this->collider = Resource::getResource<BulletColliderResource>(colliderId);
    this->rigidBody = this->collider->getNewRigidBody();
    this->rigidBody->translate(glmToBullet(this->position));
    assert_cast<BulletPhysicsProvider*>(Engine::getPhysicsProvider())->addRigidBody(this->rigidBody);
}

BulletRigidBody::BulletRigidBody(const std::string& name_, const std::string& colliderId) : Entity3d(nullptr, name_) {
    this->collider = Resource::getResource<BulletColliderResource>(colliderId);
    this->rigidBody = this->collider->getNewRigidBody();
    this->rigidBody->translate(glmToBullet(this->position));
    assert_cast<BulletPhysicsProvider*>(Engine::getPhysicsProvider())->addRigidBody(this->rigidBody);
}

BulletRigidBody::~BulletRigidBody() {
    assert_cast<BulletPhysicsProvider*>(Engine::getPhysicsProvider())->removeRigidBody(this->rigidBody);
}

void BulletRigidBody::render(const glm::mat4& parentTransform) {
    this->position = bulletToGLM(this->rigidBody->getWorldTransform().getOrigin());
    this->rotation = bulletToGLM(this->rigidBody->getWorldTransform().getRotation());
    Entity3d::render(parentTransform);
}

void BulletRigidBody::setPosition(glm::vec3 newPos) {
    btTransform newTransform;
    newTransform.setOrigin(glmToBullet(newPos));
    newTransform.setRotation(this->rigidBody->getOrientation());
    this->rigidBody->setWorldTransform(newTransform);
    this->rigidBody->getMotionState()->setWorldTransform(newTransform);
    Entity3d::setPosition(newPos);
}

void BulletRigidBody::setRotation(glm::quat newRot) {
    btTransform newTransform;
    newTransform.setOrigin(this->rigidBody->getCenterOfMassPosition());
    newTransform.setRotation(glmToBullet(newRot));
    this->rigidBody->setWorldTransform(newTransform);
    this->rigidBody->getMotionState()->setWorldTransform(newTransform);
    Entity3d::setRotation(newRot);
}

glm::vec3 BulletRigidBody::getPosition() {
    this->position = bulletToGLM(this->rigidBody->getWorldTransform().getOrigin());
    return Entity3d::getPosition();
}

glm::quat BulletRigidBody::getRotation() {
    this->rotation = bulletToGLM(this->rigidBody->getWorldTransform().getRotation());
    return Entity3d::getRotation();
}

void BulletRigidBody::translate(glm::vec3 translateByAmount) {
    this->rigidBody->translate(glmToBullet(translateByAmount));
    Entity3d::translate(translateByAmount);
}

void BulletRigidBody::rotate(glm::quat rotateByAmount) {
    btTransform newTransform;
    newTransform.setOrigin(this->rigidBody->getCenterOfMassPosition());
    newTransform.setRotation(this->rigidBody->getOrientation() + glmToBullet(rotateByAmount));
    this->rigidBody->setWorldTransform(newTransform);
    this->rigidBody->getMotionState()->setWorldTransform(newTransform);
    Entity3d::rotate(rotateByAmount);
}
