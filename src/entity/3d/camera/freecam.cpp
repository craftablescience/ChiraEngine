#include "freecam.h"

using namespace chira;

freecam::freecam(entity* parent_, const cameraProjectionMode& mode, float fov_, bool controls) : camera3d(parent_, mode, fov_) {
    if (controls)
        freecam::setupKeybinds();
}

freecam::freecam(entity* parent_, const std::string& name_, const cameraProjectionMode& mode, float fov_, bool controls) : camera3d(parent_, mode, fov_) {
    if (controls)
        freecam::setupKeybinds();
}

freecam::freecam(const cameraProjectionMode& mode, float fov_, bool controls) : camera3d(mode, fov_) {
    if (controls)
        freecam::setupKeybinds();
}

freecam::freecam(const std::string& name_, const cameraProjectionMode& mode, float fov_, bool controls) : camera3d(name_, mode, fov_) {
    if (controls)
        freecam::setupKeybinds();
}

void freecam::setupKeybinds() {
    engine::addKeybind(keybind(GLFW_KEY_W, GLFW_REPEAT,[]() {
        if (!engine::isMouseCaptured())
            return;
        if (auto cam = dynamic_cast<freecam*>(engine::getRoot()->getMainCamera()))
            cam->translateWithRotation(glm::vec3{0, 0, -cam->getMovementSpeed() * engine::getDeltaTime()});
    }));
    engine::addKeybind(keybind(GLFW_KEY_S, GLFW_REPEAT,[]() {
        if (!engine::isMouseCaptured())
            return;
        if (auto* cam = dynamic_cast<freecam*>(engine::getRoot()->getMainCamera()))
            cam->translateWithRotation(glm::vec3{0, 0, cam->getMovementSpeed() * engine::getDeltaTime()});
    }));
    engine::addKeybind(keybind(GLFW_KEY_A, GLFW_REPEAT,[]() {
        if (!engine::isMouseCaptured())
            return;
        if (auto* cam = dynamic_cast<freecam*>(engine::getRoot()->getMainCamera()))
            cam->translateWithRotation(glm::vec3{-cam->getMovementSpeed() * engine::getDeltaTime(), 0, 0});
    }));
    engine::addKeybind(keybind(GLFW_KEY_D, GLFW_REPEAT,[]() {
        if (!engine::isMouseCaptured())
            return;
        if (auto* cam = dynamic_cast<freecam*>(engine::getRoot()->getMainCamera()))
            cam->translateWithRotation(glm::vec3{cam->getMovementSpeed() * engine::getDeltaTime(), 0, 0});
    }));
    engine::addKeybind(keybind(GLFW_KEY_SPACE, GLFW_REPEAT,[]() {
        if (!engine::isMouseCaptured())
            return;
        if (auto* cam = dynamic_cast<freecam*>(engine::getRoot()->getMainCamera()))
            cam->translateWithRotation(glm::vec3{0, cam->getMovementSpeed() * engine::getDeltaTime(), 0});
    }));
    engine::addKeybind(keybind(GLFW_KEY_LEFT_SHIFT, GLFW_REPEAT,[]() {
        if (!engine::isMouseCaptured())
            return;
        if (auto* cam = dynamic_cast<freecam*>(engine::getRoot()->getMainCamera()))
            cam->translateWithRotation(glm::vec3{0, -cam->getMovementSpeed() * engine::getDeltaTime(), 0});
    }));
    engine::addMousebind(mousebind(mouseActions::MOVE, [](double xOffset, double yOffset) {
        if (!engine::isMouseCaptured())
            return;
        if (auto* cam = dynamic_cast<freecam*>(engine::getRoot()->getMainCamera())) {
            xOffset *= cam->getMouseSensitivity() * engine::getDeltaTime();
            yOffset *= cam->getMouseSensitivity() * engine::getDeltaTime();

            cam->yaw -= (float) xOffset;

            bool invertYAxis = false;
            engine::getSettingsLoader()->getValue("input", "invertYAxis", &invertYAxis);
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
    engine::addKeybind(keybind(GLFW_KEY_TAB, GLFW_PRESS, []() {
        engine::captureMouse(!engine::isMouseCaptured());
    }));
}
