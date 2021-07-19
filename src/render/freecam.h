#ifndef BASICGAMEENGINE_FREECAM_H
#define BASICGAMEENGINE_FREECAM_H


#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../utility/axis.h"
#include "../core/engine.h"
#include "perspectiveCamera.h"

class freecam : public perspectiveCamera {
public:
    explicit freecam(engine* engine, float newYaw = -90.0f, float newPitch = 0.0f, glm::vec3 newPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 currentUp = glm::vec3(0.0f, 1.0f, 0.0f), float newZoom = 45.0f, float newSpeed = 2.5f, float newSensitivity = 0.1f, bool controls = true) :
                     perspectiveCamera(engine, newYaw, newPitch, newPosition, currentUp, newZoom), movementSpeed(newSpeed), mouseSensitivity(newSensitivity) {
        this->active = true;
        if (controls) {
            this->setupKeybinds(engine);
        }
    }

    void translate(glm::vec3 offset, double delta) {
        perspectiveCamera::translate(offset, this->movementSpeed, delta);
    }

    void translateLocal(signedAxis axis, double delta) {
        perspectiveCamera::translateLocal(axis, this->movementSpeed, delta);
    }

    void setActive(bool newActive) override {
        perspectiveCamera::setActive(newActive);
        this->capturedMouse = !this->capturedMouse;
    }
protected:
    float movementSpeed;
    float mouseSensitivity;
    bool capturedMouse = true;

    void setupKeybinds(engine* engine) const {
        engine->addKeybind(keybind(GLFW_KEY_W, GLFW_REPEAT,[](class engine* e) {
            auto* cam = (freecam*) e->getCamera();
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(ZP, e->getDeltaTime());
        }));
        engine->addKeybind(keybind(GLFW_KEY_S, GLFW_REPEAT,[](class engine* e) {
            auto* cam = (freecam*) e->getCamera();
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(ZN, e->getDeltaTime());
        }));
        engine->addKeybind(keybind(GLFW_KEY_A, GLFW_REPEAT,[](class engine* e) {
            auto* cam = (freecam*) e->getCamera();
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(XP, e->getDeltaTime());
        }));
        engine->addKeybind(keybind(GLFW_KEY_D, GLFW_REPEAT,[](class engine* e) {
            auto* cam = (freecam*) e->getCamera();
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(XN, e->getDeltaTime());
        }));
        engine->addKeybind(keybind(GLFW_KEY_SPACE, GLFW_REPEAT,[](class engine* e) {
            auto* cam = (freecam*) e->getCamera();
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(YP, e->getDeltaTime());
        }));
        engine->addKeybind(keybind(GLFW_KEY_LEFT_SHIFT, GLFW_REPEAT,[](class engine* e) {
            auto* cam = (freecam*) e->getCamera();
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(YN, e->getDeltaTime());
        }));
        engine->addMousebind(mousebind(MOVE, [this](class engine* e, double xOffset, double yOffset) {
            auto* cam = (freecam*) e->getCamera();
            if (cam->isCurrent() && cam->isActive()) {
                xOffset *= this->mouseSensitivity;
                yOffset *= this->mouseSensitivity;
                cam->yaw += (float) xOffset;
                bool invertYAxis = false;
                e->getSettingsLoader()->getValue("input", "invertYAxis", &invertYAxis);
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
        }));
        engine->addKeybind(keybind(GLFW_KEY_TAB, GLFW_PRESS, [](class engine* e) {
            auto* cam = (freecam*) e->getCamera();
            if (cam->capturedMouse) {
                e->freeMouse();
                cam->setActive(false);
            } else {
                e->captureMouse();
                cam->setActive(true);
            }
        }));
    }
};


#endif //BASICGAMEENGINE_FREECAM_H