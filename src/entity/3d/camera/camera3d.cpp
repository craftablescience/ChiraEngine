#include "camera3d.h"

#include <core/engine.h>

using namespace chira;

camera3d::camera3d(entity* parent_, const cameraProjectionMode& mode, float fov_) : entity3d(parent_), fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(camera3d::getWindowWidth(), camera3d::getWindowHeight());
}

camera3d::camera3d(entity* parent_, const std::string& name_, const cameraProjectionMode& mode, float fov_) : entity3d(parent_, name_), fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(camera3d::getWindowWidth(), camera3d::getWindowHeight());
}

camera3d::camera3d(const cameraProjectionMode& mode, float fov_) : entity3d(), fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(camera3d::getWindowWidth(), camera3d::getWindowHeight());
}

camera3d::camera3d(const std::string& name_, const cameraProjectionMode& mode, float fov_) : entity3d(name_), fov(fov_) {
    this->projectionMode = mode;
    this->createProjection(camera3d::getWindowWidth(), camera3d::getWindowHeight());
}

void camera3d::createProjection(int windowWidth, int windowHeight) {
    switch (this->projectionMode) {
        case cameraProjectionMode::PERSPECTIVE:
            this->projection = glm::perspective(glm::radians(this->fov / 2), (float) windowWidth / (float) windowHeight, 0.1f, 1024.0f);
            break;
        case cameraProjectionMode::ORTHOGONAL:
            // todo
            break;
    }
}

int camera3d::getWindowWidth() {
    int windowWidth = 1600;
    engine::getSettingsLoader()->getValue("graphics", "windowWidth", &windowWidth);
    return windowWidth;
}

int camera3d::getWindowHeight() {
    int windowHeight = 900;
    engine::getSettingsLoader()->getValue("graphics", "windowHeight", &windowHeight);
    return windowHeight;
}
