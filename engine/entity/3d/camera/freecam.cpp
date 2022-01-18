#include "freecam.h"

#include <core/engine.h>
#include <input/inputManager.h>
#include <utility/debug/assertions.h>

using namespace chira;

bool Freecam::getActive() const {
    return this->active && Engine::isMouseCaptured();
}

void Freecam::setActive(bool active_) {
    Engine::captureMouse(active_);
    this->active = active_;
}

void Freecam::setupKeybinds() {
    InputManager::addCallback(InputKeyButton{Key::W, InputKeyEventType::REPEAT, []{
        if (auto cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, 0, -cam->getMovementSpeed() * Engine::getDeltaTime()});
    }});
    InputManager::addCallback(InputKeyButton{Key::S, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, 0, cam->getMovementSpeed() * Engine::getDeltaTime()});
    }});
    InputManager::addCallback(InputKeyButton{Key::A, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{-cam->getMovementSpeed() * Engine::getDeltaTime(), 0, 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::D, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{cam->getMovementSpeed() * Engine::getDeltaTime(), 0, 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::SPACE, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, cam->getMovementSpeed() * Engine::getDeltaTime(), 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::LEFT_SHIFT, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, -cam->getMovementSpeed() * Engine::getDeltaTime(), 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::TAB, InputKeyEventType::PRESSED, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()))
            cam->setActive(!cam->getActive());
    }});
    InputManager::addCallback(InputMouseMovement{InputMouseMovementEventType::MOVE, [](double xOffset, double yOffset) {
        if (auto* cam = assert_cast<Freecam*>(Engine::getRoot()->getCamera()); cam && cam->getActive()) {
            xOffset *= cam->getMouseSensitivity() * Engine::getDeltaTime();
            yOffset *= cam->getMouseSensitivity() * Engine::getDeltaTime();

            cam->yaw -= static_cast<float>(xOffset);

            bool invertYAxis = false;
            Engine::getSettingsLoader()->getValue("input", "invertYAxis", &invertYAxis);
            if (invertYAxis)
                cam->pitch -= static_cast<float>(yOffset);
            else
                cam->pitch += static_cast<float>(yOffset);

            if (cam->pitch > 89.8f)
                cam->pitch = 89.8f;
            else if (cam->pitch < -89.8f)
                cam->pitch = -89.8f;
        }
    }});
}
