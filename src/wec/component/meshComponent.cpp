#include "meshComponent.h"

meshComponent::meshComponent(mesh* mesh_, const glm::vec3& pos, const glm::vec3& rot) : positionComponent(pos), rotationComponent(rot) {
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

void meshComponent::stop() {
    this->meshPtr->release();
}
