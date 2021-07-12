#ifndef BASICGAMEENGINE_FREECAM_H
#define BASICGAMEENGINE_FREECAM_H


#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
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
                    float screenWidth,
                    float screenHeight,
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
        this->projection = glm::perspective(glm::radians(cam_ZOOM), screenWidth / screenHeight, 0.1f, 1024.0f);
        this->updateCameraVectors();

        keybind f(GLFW_KEY_W, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(0.0f, 0.0f, -1.0f), e->getDeltaTime());});
        keybind b(GLFW_KEY_S, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(0.0f, 0.0f, 1.0f), e->getDeltaTime());});
        keybind l(GLFW_KEY_A, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(-1.0f, 0.0f, 0.0f), e->getDeltaTime());});
        keybind r(GLFW_KEY_D, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(1.0f, 0.0f, 0.0f), e->getDeltaTime());});
        keybind u(GLFW_KEY_SPACE, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(0.0f, 1.0f, 0.0f), e->getDeltaTime());});
        keybind d(GLFW_KEY_LEFT_SHIFT, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(0.0f, -1.0f, 0.0f), e->getDeltaTime());});
        engine->addKeybind(f);
        engine->addKeybind(b);
        engine->addKeybind(l);
        engine->addKeybind(r);
        engine->addKeybind(u);
        engine->addKeybind(d);
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

        keybind f(GLFW_KEY_W, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(0.0f, 0.0f, -1.0f), e->getDeltaTime());});
        keybind b(GLFW_KEY_S, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(0.0f, 0.0f, 1.0f), e->getDeltaTime());});
        keybind l(GLFW_KEY_A, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(-1.0f, 0.0f, 0.0f), e->getDeltaTime());});
        keybind r(GLFW_KEY_D, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(1.0f, 0.0f, 0.0f), e->getDeltaTime());});
        keybind u(GLFW_KEY_SPACE, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(0.0f, 1.0f, 0.0f), e->getDeltaTime());});
        keybind d(GLFW_KEY_LEFT_SHIFT, GLFW_REPEAT,
                  [](class engine* e) -> void {e->getCamera()->translate(glm::vec3(0.0f, -1.0f, 0.0f), e->getDeltaTime());});
        engine->addKeybind(f);
        engine->addKeybind(b);
        engine->addKeybind(l);
        engine->addKeybind(r);
        engine->addKeybind(u);
        engine->addKeybind(d);
    }

    void translate(glm::vec3 offset, double delta) override {
        this->position += offset * (float)(movementSpeed * delta);
    }

    void rotate(float rotation, axis rotationAxis, double delta) override {
        switch (rotationAxis) {
            case axis::X:
                yaw += rotation;
                break;
            case axis::Y:
                pitch += rotation;
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
        this->view = glm::lookAt(position, position + front, up);
        return abstractCamera::getViewMatrix();
    }
    /*
    void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw   += xOffset;
        pitch += yOffset;

        if (constrainPitch) {
            if (pitch > 89.5f)
                pitch = 89.5f;
            if (pitch < -89.5f)
                pitch = -89.5f;
        }
    }
    */
private:
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
};


#endif //BASICGAMEENGINE_FREECAM_H