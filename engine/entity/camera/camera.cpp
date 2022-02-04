#include "camera.h"

#include <entity/root/window.h>
#include <physics/abstractPhysicsProvider.h>

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

AbstractRigidBody* Camera::traceRayFromCoords(glm::vec2 coords, float length) {
    const glm::vec2 windowSize = this->getFrame()->getFrameSize();

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

AbstractRigidBody* Camera::traceRayFromMouse(float length) {
    return this->traceRayFromCoords(this->getWindow()->getMousePosition(), length);
}

AbstractRigidBody* Camera::traceRayFromCenter(float length) {
    return this->traceRayFromCoords(this->getWindow()->getFrameSize() / 2, length);
}
