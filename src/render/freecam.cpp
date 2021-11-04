#include "freecam.h"

using namespace chira;

freecam::freecam(float newYaw, float newPitch, glm::vec3 newPosition, glm::vec3 currentUp, float newZoom, float newSpeed, float newSensitivity, bool controls) :
                 perspectiveCamera(newYaw, newPitch, newPosition, currentUp, newZoom), movementSpeed(newSpeed), mouseSensitivity(newSensitivity) {
    if (controls) {
        freecam::setupKeybinds();
    }
}

void freecam::init() {
    perspectiveCamera::init();
    this->setActive(engine::isMouseCaptured());
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

void freecam::setupKeybinds() const {
    // todo
    /*
    engine::addKeybind(keybind(GLFW_KEY_W, GLFW_REPEAT,[]() {
        if (engine::getMainCamera()->isCurrent() && engine::getMainCamera()->isActive()) {
            if (auto* cam = dynamic_cast<freecam*>(engine::getMainCamera())) {
                cam->translateLocal(ZP, engine::getDeltaTime());
            }
        }
    }));
    engine::addKeybind(keybind(GLFW_KEY_S, GLFW_REPEAT,[]() {
        if (engine::getMainCamera()->isCurrent() && engine::getMainCamera()->isActive()) {
            if (auto* cam = dynamic_cast<freecam*>(engine::getMainCamera())) {
                cam->translateLocal(ZN, engine::getDeltaTime());
            }
        }
    }));
    engine::addKeybind(keybind(GLFW_KEY_A, GLFW_REPEAT,[]() {
        if (engine::getMainCamera()->isCurrent() && engine::getMainCamera()->isActive()) {
            if (auto* cam = dynamic_cast<freecam*>(engine::getMainCamera())) {
                cam->translateLocal(XP, engine::getDeltaTime());
            }
        }
    }));
    engine::addKeybind(keybind(GLFW_KEY_D, GLFW_REPEAT,[]() {
        if (engine::getMainCamera()->isCurrent() && engine::getMainCamera()->isActive()) {
            if (auto* cam = dynamic_cast<freecam*>(engine::getMainCamera())) {
                cam->translateLocal(XN, engine::getDeltaTime());
            }
        }
    }));
    engine::addKeybind(keybind(GLFW_KEY_SPACE, GLFW_REPEAT,[]() {
        if (engine::getMainCamera()->isCurrent() && engine::getMainCamera()->isActive()) {
            if (auto* cam = dynamic_cast<freecam*>(engine::getMainCamera())) {
                cam->translateLocal(YP, engine::getDeltaTime());
            }
        }
    }));
    engine::addKeybind(keybind(GLFW_KEY_LEFT_SHIFT, GLFW_REPEAT,[]() {
        if (engine::getMainCamera()->isCurrent() && engine::getMainCamera()->isActive()) {
            if (auto* cam = dynamic_cast<freecam*>(engine::getMainCamera())) {
                cam->translateLocal(YN, engine::getDeltaTime());
            }
        }
    }));
    engine::addMousebind(mousebind(MOVE, [=](double xOffset, double yOffset) {
        if (engine::getMainCamera()->isCurrent() && engine::getMainCamera()->isActive()) {
            if (auto* cam = dynamic_cast<freecam*>(engine::getMainCamera())) {
                xOffset *= this->mouseSensitivity;
                yOffset *= this->mouseSensitivity;
                cam->yaw += (float) xOffset;
                bool invertYAxis = false;
                engine::getSettingsLoader()->getValue("input", "invertYAxis", &invertYAxis);
                if (invertYAxis) {
                    cam->pitch += (float) yOffset;
                } else {
                    cam->pitch -= (float) yOffset;
                }
                if (cam->pitch > 89.5f)
                    cam->pitch = 89.5f;
                if (cam->pitch < -89.5f)
                    cam->pitch = -89.5f;
            }
        }
    }));
    engine::addKeybind(keybind(GLFW_KEY_TAB, GLFW_PRESS, []() {
        if (auto* cam = dynamic_cast<freecam*>(engine::getMainCamera())) {
            if (cam->isActive()) {
                engine::captureMouse(false);
                cam->setActive(false);
            } else {
                engine::captureMouse(true);
                cam->setActive(true);
            }
        }
    }));
     */
}
