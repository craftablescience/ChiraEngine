#pragma once

#include <entity/3d/entity3d.h>

namespace chira {
    enum class cameraProjectionMode {
        PERSPECTIVE,
        ORTHOGONAL
    };

    class camera3d : public entity3d {
    public:
        camera3d(entity* parent_, const cameraProjectionMode& mode, float fov_ = 90.0f);
        camera3d(entity* parent_, const std::string& name_, const cameraProjectionMode& mode, float fov_ = 90.0f);
        explicit camera3d(const cameraProjectionMode& mode, float fov_ = 90.0f);
        explicit camera3d(const std::string& name_, const cameraProjectionMode& mode, float fov_ = 90.0f);
        void createProjection(int windowWidth, int windowHeight);
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
    protected:
        glm::mat4 projection{};
        cameraProjectionMode projectionMode;
        float fov = 90.0f;
        static int getWindowWidth();
        static int getWindowHeight();
    };
}
