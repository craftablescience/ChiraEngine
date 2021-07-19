#ifndef BASICGAMEENGINE_PERSPECTIVECAMERA_H
#define BASICGAMEENGINE_PERSPECTIVECAMERA_H


#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../utility/axis.h"
#include "../core/engine.h"
#include "abstractCamera.h"

// Largely taken from https://learnopengl.com/Getting-started/Camera
class perspectiveCamera : public abstractCamera {
public:
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
    explicit perspectiveCamera(engine* engine, float newYaw, float newPitch, glm::vec3 newPosition, glm::vec3 currentUp = glm::vec3(0.0f, 1.0f, 0.0f), float newZoom = 45.0f) :
                               yaw{newYaw}, pitch{newPitch}, position{newPosition}, worldUp{currentUp}, front{glm::vec3(0.0f, 0.0f, -1.0f)}, zoom{newZoom} {
        int windowWidth = 1600;
        engine->getSettingsLoader()->getValue("graphics", "windowWidth", &windowWidth);
        int windowHeight = 900;
        engine->getSettingsLoader()->getValue("graphics", "windowHeight", &windowHeight);
        this->projection = glm::perspective(glm::radians(this->zoom), (float) windowWidth / (float) windowHeight, 0.1f, 1024.0f);
        this->updateCameraVectors();
    }
#pragma clang diagnostic pop

    void translate(glm::vec3 offset, float amount, double delta) override {
        this->position += offset * (float)(amount * delta);
    }

    void translateLocal(signedAxis axis, float amount, double delta) {
        switch (axis) {
            case signedAxis::XP:
                this->position -= glm::normalize(glm::cross(this->front, this->up)) * amount * (float) delta;
                break;
            case signedAxis::XN:
                this->position += glm::normalize(glm::cross(this->front, this->up)) * amount * (float) delta;
                break;
            case signedAxis::YP:
                this->position += amount * this->up * (float) delta;
                break;
            case signedAxis::YN:
                this->position -= amount * this->up * (float) delta;
                break;
            case signedAxis::ZP:
                this->position += amount * this->front * (float) delta;
                break;
            case signedAxis::ZN:
                this->position -= amount * this->front * (float) delta;
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

    void setPosition(glm::vec3 newPosition) override {
        this->position = newPosition;
    }

    void setRotation(glm::vec3 rotation) override {
        this->yaw = rotation.x;
        this->pitch = rotation.y;
        this->updateCameraVectors();
    }

    void setScreenDimensions(int width, int height) override {
        this->projection = glm::perspective(glm::radians(this->zoom), (float)(width) / (float)(height), 0.1f, 1024.0f);
    }

    glm::mat4* getProjectionMatrix() override {
        return abstractCamera::getProjectionMatrix();
    }

    glm::mat4* getViewMatrix() override {
        this->updateCameraVectors();
        this->view = glm::lookAt(this->position, this->position + this->front, this->up);
        return abstractCamera::getViewMatrix();
    }

protected:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float zoom;

    void updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        newFront.y = sin(glm::radians(this->pitch));
        newFront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->front = glm::normalize(newFront);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
};


#endif //BASICGAMEENGINE_PERSPECTIVECAMERA_H