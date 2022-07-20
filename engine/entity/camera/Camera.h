#pragma once

#include <entity/Entity.h>

namespace chira {

enum class CameraProjectionMode {
    PERSPECTIVE,
    //ORTHOGRAPHIC,
};

class Camera : public Entity {
public:
    Camera(std::string name_, CameraProjectionMode mode, float fov_ = 90.f);
    explicit Camera(CameraProjectionMode mode, float fov_ = 90.f);
    void createProjection(glm::vec2 windowSize);
    [[nodiscard]] const glm::mat4& getProjection() const {
        return this->projection;
    }
    [[nodiscard]] glm::mat4 getView() {
        return glm::lookAt(this->position, this->position + this->getFrontVector(), this->getUpVector());
    }
    void setFieldOfView(float fov_) {
        this->fov = fov_;
    }
    [[nodiscard]] float getFieldOfView() const {
        return this->fov;
    }
    [[nodiscard]] glm::vec3 getFrontVector() {
        glm::vec3 out;
        out.x =     2 * (this->getRotation().x * this->getRotation().z + this->getRotation().w * this->getRotation().y);
        out.y =     2 * (this->getRotation().y * this->getRotation().z - this->getRotation().w * this->getRotation().x);
        out.z = 1 - 2 * (this->getRotation().x * this->getRotation().x + this->getRotation().y * this->getRotation().y);
        return -out; // negating for OpenGL
    }
    [[nodiscard]] glm::vec3 getUpVector() {
        glm::vec3 out;
        out.x =     2 * (this->getRotation().x * this->getRotation().y - this->getRotation().w * this->getRotation().z);
        out.y = 1 - 2 * (this->getRotation().x * this->getRotation().x + this->getRotation().z * this->getRotation().z);
        out.z =     2 * (this->getRotation().y * this->getRotation().z + this->getRotation().w * this->getRotation().x);
        return out;
    }
    [[nodiscard]] glm::vec3 getRightVector() {
        glm::vec3 out;
        out.x = 1 - 2 * (this->getRotation().y * this->getRotation().y + this->getRotation().z * this->getRotation().z);
        out.y =     2 * (this->getRotation().x * this->getRotation().y + this->getRotation().w * this->getRotation().z);
        out.z =     2 * (this->getRotation().x * this->getRotation().z - this->getRotation().w * this->getRotation().y);
        return out;
    }
protected:
    glm::mat4 projection{};
    CameraProjectionMode projectionMode;
    float fov = 90.0f;
};

} // namespace chira
