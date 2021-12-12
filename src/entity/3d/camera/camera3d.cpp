#include "camera3d.h"

#include <core/engine.h>

using namespace chira;

camera3d::camera3d(entity* parent_, const cameraProjectionMode& mode, float fov_) : entity3d(parent_), fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(engine::getWindowWidth(), engine::getWindowHeight());
}

camera3d::camera3d(entity* parent_, const std::string& name_, const cameraProjectionMode& mode, float fov_) : entity3d(parent_, name_), fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(engine::getWindowWidth(), engine::getWindowHeight());
}

camera3d::camera3d(const cameraProjectionMode& mode, float fov_) : entity3d(), fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(engine::getWindowWidth(), engine::getWindowHeight());
}

camera3d::camera3d(const std::string& name_, const cameraProjectionMode& mode, float fov_) : entity3d(name_), fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(engine::getWindowWidth(), engine::getWindowHeight());
}

void camera3d::createProjection(int windowWidth, int windowHeight) {
    switch (this->projectionMode) {
        case cameraProjectionMode::PERSPECTIVE:
            this->projection = glm::perspective(glm::radians(this->fov / 2), (float) windowWidth / (float) windowHeight, 0.1f, 1024.0f);
            break;
        //case cameraProjectionMode::ORTHOGONAL:
        //    this->projection = glm::ortho(0.f, (float) windowWidth, (float) windowHeight, 0.f, 0.1f, 1024.f);
        //    break;
    }
}
