#include "editorCamera3d.h"

#include <core/engine.h>
#include <utility/pointer/assert_cast.h>

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
    Engine::addKeybind(Keybind(GLFW_KEY_W, GLFW_REPEAT, []{
        if (auto cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, 0, -cam->getMovementSpeed() * Engine::getDeltaTime()});
    }));
    Engine::addKeybind(Keybind(GLFW_KEY_S, GLFW_REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, 0, cam->getMovementSpeed() * Engine::getDeltaTime()});
    }));
    Engine::addKeybind(Keybind(GLFW_KEY_A, GLFW_REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{-cam->getMovementSpeed() * Engine::getDeltaTime(), 0, 0});
    }));
    Engine::addKeybind(Keybind(GLFW_KEY_D, GLFW_REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{cam->getMovementSpeed() * Engine::getDeltaTime(), 0, 0});
    }));
    Engine::addKeybind(Keybind(GLFW_KEY_SPACE, GLFW_REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, cam->getMovementSpeed() * Engine::getDeltaTime(), 0});
    }));
    Engine::addKeybind(Keybind(GLFW_KEY_LEFT_SHIFT, GLFW_REPEAT, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam && cam->getActive())
            cam->translateWithRotation(glm::vec3{0, -cam->getMovementSpeed() * Engine::getDeltaTime(), 0});
    }));
    Engine::addMousebind(Mousebind(MouseActions::MOVE, [](double xOffset, double yOffset) {
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
    }));
    Engine::addKeybind(Keybind(GLFW_MOUSE_BUTTON_2, GLFW_PRESS, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam)
            cam->setActive(true);
    }, true));
    Engine::addKeybind(Keybind(GLFW_MOUSE_BUTTON_2, GLFW_RELEASE, []{
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam)
            cam->setActive(false);
    }, true));
    Engine::addMousebind(Mousebind(MouseActions::SCROLL, [](double xOffset, double yOffset) {
        if (auto* cam = assert_cast<EditorCamera3d*>(Engine::getRoot()->getMainCamera()); cam)
            cam->translateWithRotation(glm::vec3{
                xOffset * cam->getMovementSpeed() / 40,
                0,
                -yOffset * cam->getMovementSpeed() / 40 // negate for OpenGL
            });
    }));
}
