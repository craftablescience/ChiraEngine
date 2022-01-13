#include "editorCamera3d.h"

#include <core/engine.h>
#include <input/inputManager.h>
#include <utility/debug/assertions.h>

using namespace chira;

EditorCamera3d::EditorCamera3d(Entity* parent_, CameraProjectionMode mode, float fov_, bool controls) : Freecam(parent_, mode, fov_, false) {
    if (controls)
        EditorCamera3d::setupKeybinds();
}

EditorCamera3d::EditorCamera3d(Entity* parent_, const std::string& name_, CameraProjectionMode mode, float fov_, bool controls) : Freecam(parent_, mode, fov_, false) {
    if (controls)
        EditorCamera3d::setupKeybinds();
}

EditorCamera3d::EditorCamera3d(CameraProjectionMode mode, float fov_, bool controls) : Freecam(mode, fov_, false) {
    if (controls)
        EditorCamera3d::setupKeybinds();
}

EditorCamera3d::EditorCamera3d(const std::string& name_, CameraProjectionMode mode, float fov_, bool controls) : Freecam(name_, mode, fov_, false) {
    if (controls)
        EditorCamera3d::setupKeybinds();
}

void EditorCamera3d::setupKeybinds() {
    InputManager::addCallback(InputKeyButton{Key::W, InputKeyEventType::REPEAT, []{
        if (auto cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, 0, -cam->getMovementSpeed() * Engine::getDeltaTime()});
    }});
    InputManager::addCallback(InputKeyButton{Key::S, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, 0, cam->getMovementSpeed() * Engine::getDeltaTime()});
    }});
    InputManager::addCallback(InputKeyButton{Key::A, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{-cam->getMovementSpeed() * Engine::getDeltaTime(), 0, 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::D, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{cam->getMovementSpeed() * Engine::getDeltaTime(), 0, 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::SPACE, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, cam->getMovementSpeed() * Engine::getDeltaTime(), 0});
    }});
    InputManager::addCallback(InputKeyButton{Key::LEFT_SHIFT, InputKeyEventType::REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, -cam->getMovementSpeed() * Engine::getDeltaTime(), 0});
    }});
    InputManager::addCallback(InputMouseButton{Key::MOUSE_RIGHT, InputKeyEventType::PRESSED, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam)
            cam->setActive(true);
    }});
    InputManager::addCallback(InputMouseButton{Key::MOUSE_RIGHT, InputKeyEventType::RELEASED, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam)
            cam->setActive(false);
    }});
    InputManager::addCallback(InputMouseMovement{InputMouseMovementEventType::MOVE, [](double xOffset, double yOffset) {
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive()) {
            xOffset *= cam->getMouseSensitivity() * Engine::getDeltaTime();
            yOffset *= cam->getMouseSensitivity() * Engine::getDeltaTime();

            cam->yaw -= (float) xOffset;

            bool invertYAxis = false;
            Engine::getSettingsLoader()->getValue("input", "invertYAxis", &invertYAxis);
            if (invertYAxis)
                cam->pitch -= (float) yOffset;
            else
                cam->pitch += (float) yOffset;

            if (cam->pitch > 89.8f)
                cam->pitch = 89.8f;
            else if (cam->pitch < -89.8f)
                cam->pitch = -89.8f;
        }
    }});
    InputManager::addCallback(InputMouseMovement{InputMouseMovementEventType::SCROLL, [](double x, double y) {
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam)
            cam->translateWithRotation(glm::vec3{
                x * cam->getMovementSpeed() / 40,
                0,
                -y * cam->getMovementSpeed() / 40 // negate for OpenGL
            });
    }});
}
