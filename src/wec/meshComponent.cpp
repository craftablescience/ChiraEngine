#include "meshComponent.h"

using namespace chira;

meshComponent::meshComponent(meshResource* mesh_, const glm::vec3& pos, const glm::quat& rot) : positionComponentData(pos), rotationComponentData(rot) {
    this->meshPtr = mesh_;
}

void meshComponent::render(double delta) {
    this->meshPtr->render(&this->position, &this->rotation);
}

meshComponent::~meshComponent() {
    this->meshPtr->release();
}
