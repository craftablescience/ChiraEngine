#include "meshComponent.h"

using namespace chira;

meshComponent::meshComponent(mesh* mesh_, const glm::vec3& pos, const glm::vec3& rot) : positionComponentData(pos), rotationComponentData(rot) {
    this->meshPtr = mesh_;
    this->meshPtr->setPosition(&this->position);
    this->meshPtr->setRotation(&this->rotation);
}

void meshComponent::render(double delta) {
    /*
    if (this->positionChanged) {
        // don't need to do anything here
        this->positionChanged = false;
    }
    if (this->rotationChanged) {
        // don't need to do anything here
        this->rotationChanged = false;
    }
    */
    this->meshPtr->render();
}

meshComponent::~meshComponent() {
    this->meshPtr->release();
}
