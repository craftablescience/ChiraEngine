#pragma once

#include "camera3d.h"

namespace chira {
    class EditorCamera3d : public Camera3d {
    public:
        EditorCamera3d(Entity* parent_, CameraProjectionMode mode, float fov_ = 90.0f, bool controls = true);
        EditorCamera3d(Entity* parent_, const std::string& name_, CameraProjectionMode mode, float fov_ = 90.0f, bool controls = true);
        explicit EditorCamera3d(CameraProjectionMode mode, float fov_ = 90.0f, bool controls = true);
        EditorCamera3d(const std::string& name_, CameraProjectionMode mode, float fov_ = 90.0f, bool controls = true);
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
        [[nodiscard]] float getDistanceFromPivot() const {
            return this->distance;
        }
        void setDistanceFromPivot(float newDistance) {
            this->distance = newDistance;
        }
        void addDistanceFromPivot(float additionalDistance) {
            this->distance += additionalDistance;
            if (this->distance < 0)
                this->distance = 0;
        }
    protected:
        float movementSpeed = 4.f;
        float mouseSensitivity = 6.f;
        float pitch = 0.f;
        float yaw = 0.f;
        float distance = 0.f;
        bool capturedMouse = false;
        static void setupKeybinds();
    private:
        using Entity3d::setRotation;
        using Entity3d::rotate;
    };
}
