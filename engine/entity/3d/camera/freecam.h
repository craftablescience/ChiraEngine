#pragma once

#include "camera3d.h"

namespace chira {
    class freecam : public camera3d {
    public:
        freecam(entity* parent_, const cameraProjectionMode& mode, float fov_ = 90.0f, bool controls = true);
        freecam(entity* parent_, const std::string& name_, const cameraProjectionMode& mode, float fov_ = 90.0f, bool controls = true);
        explicit freecam(const cameraProjectionMode& mode, float fov_ = 90.0f, bool controls = true);
        explicit freecam(const std::string& name_, const cameraProjectionMode& mode, float fov_ = 90.0f, bool controls = true);
        glm::quat getRotation() override {
            return glm::angleAxis(glm::radians(this->yaw), glm::vec3(0,1,0)) * glm::angleAxis(glm::radians(-this->pitch), glm::vec3(1,0,0));
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
    protected:
        float movementSpeed = 4.f;
        float mouseSensitivity = 6.f;
        float pitch = 0.f;
        float yaw = 0.f;
        bool capturedMouse = false;
        static void setupKeybinds();
    private:
        using entity3d::setRotation;
        using entity3d::rotate;
    };
}
