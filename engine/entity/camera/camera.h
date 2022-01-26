#pragma once

#include <entity/entity.h>

namespace chira {
    class AbstractRigidBody;

    enum class CameraProjectionMode {
        PERSPECTIVE,
        //ORTHOGRAPHIC,
    };

    class Camera : public Entity {
    public:
        Camera(Entity* parent_, CameraProjectionMode mode, float fov_ = 90.0f);
        Camera(Entity* parent_, const std::string& name_, CameraProjectionMode mode, float fov_ = 90.0f);
        explicit Camera(CameraProjectionMode mode, float fov_ = 90.0f);
        Camera(const std::string& name_, CameraProjectionMode mode, float fov_ = 90.0f);
        void createProjection(glm::vec2 windowSize);
        const glm::mat4& getProjection() const {
            return this->projection;
        }
        glm::mat4 getView() {
            return glm::lookAt(this->position, this->position + this->getFrontVector(), this->getUpVector());
        }
        void setFieldOfView(float fov_) {
            this->fov = fov_;
        }
        float getFieldOfView() const {
            return this->fov;
        }
        glm::vec3 getFrontVector() {
            glm::vec3 out;
            out.x =     2 * (this->getRotation().x * this->getRotation().z + this->getRotation().w * this->getRotation().y);
            out.y =     2 * (this->getRotation().y * this->getRotation().z - this->getRotation().w * this->getRotation().x);
            out.z = 1 - 2 * (this->getRotation().x * this->getRotation().x + this->getRotation().y * this->getRotation().y);
            return -out; // negating for OpenGL
        }
        glm::vec3 getUpVector() {
            glm::vec3 out;
            out.x =     2 * (this->getRotation().x * this->getRotation().y - this->getRotation().w * this->getRotation().z);
            out.y = 1 - 2 * (this->getRotation().x * this->getRotation().x + this->getRotation().z * this->getRotation().z);
            out.z =     2 * (this->getRotation().y * this->getRotation().z + this->getRotation().w * this->getRotation().x);
            return out;
        }
        glm::vec3 getRightVector() {
            glm::vec3 out;
            out.x = 1 - 2 * (this->getRotation().y * this->getRotation().y + this->getRotation().z * this->getRotation().z);
            out.y =     2 * (this->getRotation().x * this->getRotation().y + this->getRotation().w * this->getRotation().z);
            out.z =     2 * (this->getRotation().x * this->getRotation().z - this->getRotation().w * this->getRotation().y);
            return out;
        }
        AbstractRigidBody* traceRayFromCoords(glm::vec2 coords, float length);
        AbstractRigidBody* traceRayFromMouse(float length);
        AbstractRigidBody* traceRayFromCenter(float length);
    protected:
        glm::mat4 projection{};
        CameraProjectionMode projectionMode;
        float fov = 90.0f;
    };
}
