#include "meshComponent.h"

meshComponent::meshComponent(mesh* mesh_, const glm::vec3& pos) : positionComponent(pos) {
    this->meshPtr = mesh_;
}

void meshComponent::render(double delta) {
    if (this->positionChanged) {
        // todo: set mesh position
        this->positionChanged = false;
    }
    this->meshPtr->render();
}

void meshComponent::stop() {
    this->meshPtr->release();
}
