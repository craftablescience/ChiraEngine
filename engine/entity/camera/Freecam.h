#pragma once

#include "Camera.h"

namespace chira {

class Freecam : public Camera {
public:
    Freecam(std::string name_, CameraProjectionMode mode, float fov_ = 90.f) : Camera(std::move(name_), mode, fov_) {}
    explicit Freecam(CameraProjectionMode mode, float fov_ = 90.f) : Camera(mode, fov_) {}
    glm::quat getRotation() override {
        return glm::angleAxis(glm::radians(this->yaw), glm::vec3{0,1,0}) * glm::angleAxis(glm::radians(-this->pitch), glm::vec3{1,0,0});
    }
    void setPitch(float pitch_) {
        this->pitch = pitch_;
    }
    [[nodiscard]] float getPitch() const {
        return this->pitch;
    }
    void setYaw(float yaw_) {
        this->yaw = yaw_;
    }
    [[nodiscard]] float getYaw() const {
        return this->yaw;
    }
    void setMovementSpeed(float movementSpeed_) {
        this->movementSpeed = movementSpeed_;
    }
    [[nodiscard]] float getMovementSpeed() const {
        return this->movementSpeed;
    }
    void setMouseSensitivity(float mouseSensitivity_) {
        this->mouseSensitivity = mouseSensitivity_;
    }
    [[nodiscard]] float getMouseSensitivity() const {
        return this->mouseSensitivity;
    }
    [[nodiscard]] bool getActive() const;
    void setActive(bool active_);

    //todo(input): this is dumb
    static void setupKeybinds();
protected:
    float movementSpeed    = 4.f;
    float mouseSensitivity = 6.f;
    float pitch = 0.f;
    float yaw   = 0.f;
    bool active = false;
private:
    using Entity::setRotation;
    using Entity::rotate;
};

} // namespace chira
