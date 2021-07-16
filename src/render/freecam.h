#ifndef BASICGAMEENGINE_FREECAM_H
#define BASICGAMEENGINE_FREECAM_H


#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../utility/axis.h"
#include "../core/engine.h"
#include "abstractCamera.h"

const float cam_YAW         = -90.0f;
const float cam_PITCH       =  0.0f;
const float cam_SPEED       =  2.5f;
const float cam_SENSITIVITY =  0.1f;
const float cam_ZOOM        =  45.0f;

// Largely taken from https://learnopengl.com/Getting-started/Camera
class freecam : public abstractCamera {
public:
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    explicit freecam(engine* engine,
                    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                    float yaw = cam_YAW,
                    float pitch = cam_PITCH):
                    front(glm::vec3(0.0f, 0.0f, -1.0f)),
                    movementSpeed(cam_SPEED),
                    mouseSensitivity(cam_SENSITIVITY),
                    zoom(cam_ZOOM) {
        this->position = position;
        worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        int windowWidth = 1600;
        engine->getSettingsLoader()->getValue("graphics", "windowWidth", &windowWidth);
        int windowHeight = 900;
        engine->getSettingsLoader()->getValue("graphics", "windowHeight", &windowHeight);
        this->projection = glm::perspective(glm::radians(cam_ZOOM), (float) windowWidth / (float) windowHeight, 0.1f, 1024.0f);
        this->updateCameraVectors();
        this->active = true;
        freecam::setupKeybinds(engine);
    }

    explicit freecam(engine* engine,
                    float screenWidth,
                    float screenHeight,
                    float posX = 0.0f,
                    float posY = 0.0f,
                    float posZ = 0.0f,
                    float upX = 0.0f,
                    float upY = 1.0f,
                    float upZ = 0.0f,
                    float yaw = cam_YAW,
                    float pitch = cam_PITCH):
                    front(glm::vec3(0.0f, 0.0f, -1.0f)),
                    movementSpeed(cam_SPEED),
                    mouseSensitivity(cam_SENSITIVITY),
                    zoom(cam_ZOOM) {
        position = glm::vec3(posX, posY, posZ);
        worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->projection = glm::perspective(glm::radians(cam_ZOOM), screenWidth / screenHeight, 0.1f, 1024.0f);
        this->updateCameraVectors();
        this->active = true;
        freecam::setupKeybinds(engine);
    }

    void translate(glm::vec3 offset, double delta) override {
        this->position += offset * (float)(movementSpeed * delta);
    }

    void translateLocal(signedAxis axis, double delta) {
        switch (axis) {
            case XP:
                this->position -= glm::normalize(glm::cross(this->front, this->up)) * this->movementSpeed * (float) delta;
                break;
            case XN:
                this->position += glm::normalize(glm::cross(this->front, this->up)) * this->movementSpeed * (float) delta;
                break;
            case YP:
                this->position += this->movementSpeed * this->up * (float) delta;
                break;
            case YN:
                this->position -= this->movementSpeed * this->up * (float) delta;
                break;
            case ZP:
                this->position += this->movementSpeed * this->front * (float) delta;
                break;
            case ZN:
                this->position -= this->movementSpeed * this->front * (float) delta;
                break;
        }
    }

    void rotate(float rotation, axis rotationAxis, double delta) override {
        switch (rotationAxis) {
            case axis::X:
                yaw += rotation * (float) delta;
                break;
            case axis::Y:
                pitch += rotation * (float) delta;
                break;
            case axis::Z:
            default:
                break;
        }
        this->updateCameraVectors();
    }

    void setPosition(glm::vec3 position) override {
        this->position = position;
    }

    void setRotation(glm::vec3 rotation) override {
        this->yaw   = rotation.x;
        this->pitch = rotation.y;
    }

    void setScreenDimensions(int width, int height) override {
        this->projection = glm::perspective(glm::radians(cam_ZOOM), (float)(width) / (float)(height), 0.1f, 1024.0f);
    }

    glm::mat4* getProjectionMatrix() override {
        return abstractCamera::getProjectionMatrix();
    }

    glm::mat4* getViewMatrix() override {
        this->updateCameraVectors();
        this->view = glm::lookAt(position, position + front, up);
        return abstractCamera::getViewMatrix();
    }
    void setActive(bool newActive) override {
        abstractCamera::setActive(newActive);
        this->capturedMouse = !this->capturedMouse;
    }
private:
    bool capturedMouse = true;
    void updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        // also, re-calculate the right and up vector
        right = glm::normalize(glm::cross(front, worldUp));
        up    = glm::normalize(glm::cross(right, front));
    }

    static void setupKeybinds(engine* engine) {
        keybind f(GLFW_KEY_W, GLFW_REPEAT,[](class engine* e) {
            auto* cam = dynamic_cast<freecam*>(e->getCamera());
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(ZP, e->getDeltaTime());
        });
        keybind b(GLFW_KEY_S, GLFW_REPEAT,[](class engine* e) {
            auto* cam = dynamic_cast<freecam*>(e->getCamera());
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(ZN, e->getDeltaTime());
        });
        keybind l(GLFW_KEY_A, GLFW_REPEAT,[](class engine* e) {
            auto* cam = dynamic_cast<freecam*>(e->getCamera());
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(XP, e->getDeltaTime());
        });
        keybind r(GLFW_KEY_D, GLFW_REPEAT,[](class engine* e) {
            auto* cam = dynamic_cast<freecam*>(e->getCamera());
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(XN, e->getDeltaTime());
        });
        keybind u(GLFW_KEY_SPACE, GLFW_REPEAT,[](class engine* e) {
            auto* cam = dynamic_cast<freecam*>(e->getCamera());
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(YP, e->getDeltaTime());
        });
        keybind d(GLFW_KEY_LEFT_SHIFT, GLFW_REPEAT,[](class engine* e) {
            auto* cam = dynamic_cast<freecam*>(e->getCamera());
            if (cam->isCurrent() && cam->isActive()) cam->translateLocal(YN, e->getDeltaTime());
        });
        engine->addKeybind(f);
        engine->addKeybind(b);
        engine->addKeybind(l);
        engine->addKeybind(r);
        engine->addKeybind(u);
        engine->addKeybind(d);
        mousebind look(MOVE, [](class engine* e, double xOffset, double yOffset) {
            auto* cam = dynamic_cast<freecam*>(e->getCamera());
            if (cam->isCurrent() && cam->isActive()) {
                xOffset *= cam_SENSITIVITY;
                yOffset *= cam_SENSITIVITY;
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
        });
        engine->addMousebind(look);
        keybind tab(GLFW_KEY_TAB, GLFW_PRESS, [](class engine* e) {
            if (dynamic_cast<freecam*>(e->getCamera())->capturedMouse) {
                e->freeMouse();
                dynamic_cast<freecam*>(e->getCamera())->setActive(false);
            } else {
                e->captureMouse();
                dynamic_cast<freecam*>(e->getCamera())->setActive(true);
            }
        });
        engine->addKeybind(tab);
    }
};


#endif //BASICGAMEENGINE_FREECAM_H