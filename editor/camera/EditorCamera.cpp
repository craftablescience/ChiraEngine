#include "EditorCamera.h"

#include <config/ConEntry.h>
#include <core/Assertions.h>
#include <core/Engine.h>
#include <input/InputManager.h>

using namespace chira;

void EditorCamera::setupKeybinds() {
    InputManager::addCallback(InputKeyButton{Key::W, InputKeyEventType::REPEAT, []{
        if (auto cam = assert_cast<EditorCamera*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, 0, -cam->getMovementSpeed() * Engine::getDeltaTime()});
    }});
    InputManager::addCallback(InputKeyButton{Key::S, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, 0, cam->getMovementSpeed() * Engine::getDeltaTime()});
    }});
    InputManager::addCallback(InputKeyButton{Key::A, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({-cam->getMovementSpeed() * Engine::getDeltaTime(), 0, 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::D, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({cam->getMovementSpeed() * Engine::getDeltaTime(), 0, 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::SPACE, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, cam->getMovementSpeed() * Engine::getDeltaTime(), 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::LEFT_SHIFT, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({0, -cam->getMovementSpeed() * Engine::getDeltaTime(), 0});
    }});
    InputManager::addCallback(InputMouseMovement{InputMouseMovementEventType::MOVE, [](double xOffset, double yOffset) {
        if (auto* cam = assert_cast<EditorCamera*>(Engine::getWindow()->getCamera()); cam && cam->getActive()) {
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
    InputManager::addCallback(InputMouseMovement{InputMouseMovementEventType::SCROLL, [](double x, double y) {
        if (auto* cam = assert_cast<EditorCamera*>(Engine::getWindow()->getCamera()); cam && cam->getActive())
            cam->translateWithRotation({
                x * cam->getMovementSpeed() / 40,
                0,
                -y * cam->getMovementSpeed() / 40 // negate for OpenGL
            });
    }});
}
