#include "Freecam.h"

#include <config/ConEntry.h>
#include <core/Assertions.h>
#include <core/Engine.h>
#include <input/InputManager.h>

using namespace chira;

bool Freecam::getActive() const {
    return this->active && Engine::getWindow()->isMouseCaptured();
}

void Freecam::setActive(bool active_) {
    Engine::getWindow()->captureMouse(active_);
    this->active = active_;
}

void Freecam::setupKeybinds() {
    InputManager::addCallback(InputKeyButton{Key::W, InputKeyEventType::REPEAT, []{
        if (auto cam = assert_cast<Freecam*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, 0, -cam->getMovementSpeed() * Engine::getDeltaTime()});
    }});
    InputManager::addCallback(InputKeyButton{Key::S, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, 0, cam->getMovementSpeed() * Engine::getDeltaTime()});
    }});
    InputManager::addCallback(InputKeyButton{Key::A, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({-cam->getMovementSpeed() * Engine::getDeltaTime(), 0, 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::D, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({cam->getMovementSpeed() * Engine::getDeltaTime(), 0, 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::SPACE, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, cam->getMovementSpeed() * Engine::getDeltaTime(), 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::LEFT_SHIFT, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, -cam->getMovementSpeed() * Engine::getDeltaTime(), 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::TAB, InputKeyEventType::PRESSED, []{
        if (auto* cam = assert_cast<Freecam*>(Engine::getWindow()->getCamera()))
            cam->setActive(!cam->getActive());
    }});
    InputManager::addCallback(InputMouseMovement{InputMouseMovementEventType::MOVE, [](double xOffset, double yOffset) {
        if (auto* cam = assert_cast<Freecam*>(Engine::getWindow()->getCamera()); cam && cam->getActive()) {
            xOffset *= cam->getMouseSensitivity() * Engine::getDeltaTime();
            yOffset *= cam->getMouseSensitivity() * Engine::getDeltaTime();

            if (ConVarRegistry::hasConVar("input_invert_x_axis") && ConVarRegistry::getConVar("input_invert_x_axis")->getValue<bool>())
                cam->yaw += static_cast<float>(xOffset);
            else
                cam->yaw -= static_cast<float>(xOffset);

            if (ConVarRegistry::hasConVar("input_invert_y_axis") && ConVarRegistry::getConVar("input_invert_y_axis")->getValue<bool>())
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
