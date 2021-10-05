#include "meshComponent.h"

using namespace chira;

meshComponent::meshComponent(mesh* mesh_, const glm::vec3& pos, const glm::vec3& rot) : positionComponentData(pos), rotationComponentData(rot) {
    this->meshPtr = mesh_;
}

void meshComponent::render(double delta) {
    if (this->positionChanged) {
        // todo: set mesh position
        this->positionChanged = false;
    }
    if (this->rotationChanged) {
        // todo: set mesh rotation
        this->rotationChanged = false;
    }
    this->meshPtr->render();
}

meshComponent::~meshComponent() {
    this->meshPtr->release();
}
