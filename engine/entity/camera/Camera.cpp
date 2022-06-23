#include "Camera.h"

#include <entity/root/Window.h>

using namespace chira;

Camera::Camera(const std::string& name_, CameraProjectionMode mode, float fov_)
    : Entity(name_)
    , fov(fov_) {
    this->projectionMode = mode;
}

Camera::Camera(CameraProjectionMode mode, float fov_)
    : Entity()
    , fov(fov_) {
    this->projectionMode = mode;
}

void Camera::createProjection(glm::vec2 windowSize) {
    if (windowSize.x <= 0 || windowSize.y <= 0)
        return;
    switch (this->projectionMode) {
        case CameraProjectionMode::PERSPECTIVE:
            this->projection = glm::perspective(glm::radians(this->fov / 2), windowSize.x / windowSize.y, 0.1f, 1024.0f);
            break;
        //case CameraProjectionMode::ORTHOGONAL:
        //    this->projection = glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, 0.1f, 1024.f);
        //    break;
    }
}
