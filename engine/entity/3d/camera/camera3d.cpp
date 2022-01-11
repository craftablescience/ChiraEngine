#include "camera3d.h"

#include <core/engine.h>

using namespace chira;

Camera3d::Camera3d(Entity* parent_, CameraProjectionMode mode, float fov_)
    : Entity3d(parent_)
    , fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(Engine::getWindowWidth(), Engine::getWindowHeight());
}

Camera3d::Camera3d(Entity* parent_, const std::string& name_, CameraProjectionMode mode, float fov_)
    : Entity3d(parent_, name_)
    , fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(Engine::getWindowWidth(), Engine::getWindowHeight());
}

Camera3d::Camera3d(CameraProjectionMode mode, float fov_)
    : Entity3d()
    , fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(Engine::getWindowWidth(), Engine::getWindowHeight());
}

Camera3d::Camera3d(const std::string& name_, CameraProjectionMode mode, float fov_)
    : Entity3d(name_)
    , fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(Engine::getWindowWidth(), Engine::getWindowHeight());
}

void Camera3d::createProjection(int windowWidth, int windowHeight) {
    switch (this->projectionMode) {
        case CameraProjectionMode::PERSPECTIVE:
            this->projection = glm::perspective(glm::radians(this->fov / 2), (float) windowWidth / (float) windowHeight, 0.1f, 1024.0f);
            break;
        //case CameraProjectionMode::ORTHOGONAL:
        //    this->projection = glm::ortho(0.f, (float) windowWidth, (float) windowHeight, 0.f, 0.1f, 1024.f);
        //    break;
    }
}
