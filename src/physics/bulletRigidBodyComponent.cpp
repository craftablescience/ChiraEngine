#include "bulletRigidBodyComponent.h"

#include "../resource/resourceManager.h"
#include "../core/engine.h"
#include "bulletPhysicsProvider.h"

using namespace chira;

bulletRigidBodyComponent::bulletRigidBodyComponent(const std::string& identifier, const glm::vec3& transform) {
    this->collider = resourceManager::getResource<bulletColliderResource>(identifier);
    this->rigidBody = this->collider->getNewRigidBody();
    this->rigidBody->translate(bulletRigidBodyComponent::glmToBt(transform));
    dynamic_cast<bulletPhysicsProvider*>(engine::getPhysicsProvider())->addRigidBody(this->rigidBody);
}
