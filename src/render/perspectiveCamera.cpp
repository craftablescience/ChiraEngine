#include "perspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>

#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
perspectiveCamera::perspectiveCamera(engine* engine, float newYaw, float newPitch, glm::vec3 newPosition, glm::vec3 currentUp, float newZoom) :
                                     yaw{newYaw}, pitch{newPitch}, position{newPosition}, worldUp{currentUp}, front{glm::vec3(0.0f, 0.0f, -1.0f)}, zoom{newZoom} {
    int windowWidth = 1600;
    engine->getSettingsLoader()->getValue("graphics", "windowWidth", &windowWidth);
    int windowHeight = 900;
    engine->getSettingsLoader()->getValue("graphics", "windowHeight", &windowHeight);
    this->projection = glm::perspective(glm::radians(this->zoom), (float) windowWidth / (float) windowHeight, 0.1f, 1024.0f);
    this->updateCameraVectors();
}

void perspectiveCamera::translate(glm::vec3 offset, float amount, double delta) {
    this->position += offset * (float)(amount * delta);
}

void perspectiveCamera::translateLocal(signedAxis axis, float amount, double delta) {
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

void perspectiveCamera::rotate(float rotation, axis rotationAxis, double delta) {
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

void perspectiveCamera::setPosition(glm::vec3 newPosition) {
    this->position = newPosition;
}

void perspectiveCamera::setRotation(glm::vec3 rotation) {
    this->yaw = rotation.x;
    this->pitch = rotation.y;
    this->updateCameraVectors();
}

void perspectiveCamera::setScreenDimensions(int width, int height) {
    this->projection = glm::perspective(glm::radians(this->zoom), (float)(width) / (float)(height), 0.1f, 1024.0f);
}

glm::mat4* perspectiveCamera::getViewMatrix() {
    this->updateCameraVectors();
    this->view = glm::lookAt(this->position, this->position + this->front, this->up);
    return abstractCamera::getViewMatrix();
}

void perspectiveCamera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    newFront.y = sin(glm::radians(this->pitch));
    newFront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(newFront);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
