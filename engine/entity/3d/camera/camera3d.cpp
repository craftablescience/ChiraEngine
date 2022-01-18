#include "camera3d.h"

#include "../root.h"
#include <physics/abstractPhysicsProvider.h>

using namespace chira;

Camera3d::Camera3d(Entity* parent_, CameraProjectionMode mode, float fov_)
    : Entity3d(parent_)
    , fov(fov_) {
    this->projectionMode = mode;
}

Camera3d::Camera3d(Entity* parent_, const std::string& name_, CameraProjectionMode mode, float fov_)
    : Entity3d(parent_, name_)
    , fov(fov_) {
    this->projectionMode = mode;
}

Camera3d::Camera3d(CameraProjectionMode mode, float fov_)
    : Entity3d()
    , fov(fov_) {
    this->projectionMode = mode;
}

Camera3d::Camera3d(const std::string& name_, CameraProjectionMode mode, float fov_)
    : Entity3d(name_)
    , fov(fov_) {
    this->projectionMode = mode;
}

void Camera3d::createProjection(glm::vec2 windowSize) {
    switch (this->projectionMode) {
        case CameraProjectionMode::PERSPECTIVE:
            this->projection = glm::perspective(glm::radians(this->fov / 2), windowSize.x / windowSize.y, 0.1f, 1024.0f);
            break;
        //case CameraProjectionMode::ORTHOGONAL:
        //    this->projection = glm::ortho(0.f, windowSize.x, windowSize.y, 0.f, 0.1f, 1024.f);
        //    break;
    }
}

AbstractRigidBody* Camera3d::traceRayFromCoords(glm::vec2 coords, float length) {
    glm::vec2 windowSize = this->getRoot()->getWindowSize();

    glm::vec4 rayStartNDC{
            (coords.x / windowSize.x - 0.5f) * 2.f, // [0,screenX] -> [-1,1]
            (coords.y / windowSize.y - 0.5f) * 2.f, // [0,screenY] -> [-1,1]
            -1.f, // near plane maps to Z=-1 in NDC
            1.f
    };
    glm::vec4 rayEndNDC{
            (coords.x / windowSize.x - 0.5f) * 2.f,
            (coords.y / windowSize.y - 0.5f) * 2.f,
            0.f,
            1.f
    };

    glm::mat4 invProj = glm::inverse(this->getProjection());
    glm::mat4 invView = glm::inverse(this->getView());
    glm::vec4 rayStartLocal = invProj * rayStartNDC;   rayStartLocal /= rayStartLocal.w;
    glm::vec4 rayStart      = invView * rayStartLocal; rayStart      /= rayStart.w;
    glm::vec4 rayEndLocal   = invProj * rayEndNDC;     rayEndLocal   /= rayEndLocal.w;
    glm::vec4 rayEnd        = invView * rayEndLocal;   rayEnd        /= rayEnd.w;

    glm::vec3 rayDirection = glm::normalize(glm::vec3{rayEnd - rayStart});
    return AbstractPhysicsProvider::getPhysicsProvider()->traceRay(this->getGlobalPosition(), rayDirection, length);
}

AbstractRigidBody* Camera3d::traceRayFromMouse(float length) {
    return this->traceRayFromCoords(this->getRoot()->getMousePosition(), length);
}

AbstractRigidBody* Camera3d::traceRayFromCenter(float length) {
    return this->traceRayFromCoords(this->getRoot()->getWindowSize() / 2.f, length);
}
