#pragma once

#include <glm/glm.hpp>
#include "../utility/axis.h"

class engine;

class abstractCamera {
public:
    virtual ~abstractCamera() = default;
    virtual void init(engine* engine) = 0;
    virtual void translate(glm::vec3 offset, float amount, double delta) = 0;
    virtual void rotate(float rotation, axis rotationAxis, double delta) = 0;
    virtual void setPosition(glm::vec3 position) = 0;
    virtual glm::vec3 getPosition() = 0;
    virtual void setRotation(glm::vec3 rotation) = 0;
    virtual glm::vec3 getRotation() = 0;
    virtual glm::vec3 getUpVector() = 0;
    virtual void setScreenDimensions(int width, int height) = 0;
    virtual glm::mat4* getProjectionMatrix() {
        return &(this->projection);
    }
    virtual glm::mat4* getViewMatrix() {
        return &(this->view);
    }
    [[nodiscard]] bool isCurrent() const {
        return this->current;
    }
    virtual void setCurrent(bool newCurrent) {
        this->current = newCurrent;
    }
    [[nodiscard]] bool isActive() const {
        return this->active;
    }
    virtual void setActive(bool newActive) {
        this->active = newActive;
    }
protected:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 position;
    bool current;
    bool active;
};
