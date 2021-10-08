#include "meshComponent.h"

using namespace chira;

meshComponent::meshComponent(mesh* mesh_, const glm::vec3& pos, const glm::quat& rot) : positionComponentData(pos), rotationComponentData(rot) {
    this->meshPtr = mesh_;
    this->meshPtr->setPosition(&this->position);
    this->meshPtr->setRotation(&this->rotation);
}

void meshComponent::render(double delta) {
    this->meshPtr->render();
}

meshComponent::~meshComponent() {
    this->meshPtr->release();
}
