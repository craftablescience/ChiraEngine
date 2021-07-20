#include "freecam.h"

freecam::freecam(engine* engine, float newYaw, float newPitch, glm::vec3 newPosition, glm::vec3 currentUp, float newZoom, float newSpeed, float newSensitivity, bool controls) :
                 perspectiveCamera(engine, newYaw, newPitch, newPosition, currentUp, newZoom), movementSpeed(newSpeed), mouseSensitivity(newSensitivity) {
    this->active = true;
    if (controls) {
        this->setupKeybinds(engine);
    }
}

void freecam::translate(glm::vec3 offset, double delta) {
    perspectiveCamera::translate(offset, this->movementSpeed, delta);
}

void freecam::translateLocal(signedAxis axis, double delta) {
    perspectiveCamera::translateLocal(axis, this->movementSpeed, delta);
}

void freecam::setActive(bool newActive) {
    perspectiveCamera::setActive(newActive);
    this->capturedMouse = !this->capturedMouse;
}
