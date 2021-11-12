#pragma once

#include "../entity3d.h"

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
        virtual const glm::mat4& getProjection() {
            return this->projection;
        }
        virtual glm::mat4 getView() {
            return glm::lookAt(this->position, this->position + this->getFrontVector(), this->getUpVector());
        }
        void setFieldOfView(float fov_) {
            this->fov = fov_;
        }
        float getFieldOfView() const {
            return this->fov;
        }
        glm::vec3 getFrontVector() {
            // todo
            return glm::vec3{0,0,-1};
        }
        glm::vec3 getUpVector() {
            // todo
            return glm::vec3{0,1,0};
        }
        glm::vec3 getRightVector() {
            // todo
            return glm::vec3{1,0,0};
        }
    protected:
        glm::mat4 projection{};
        cameraProjectionMode projectionMode;
        float fov = 90.0f;
        static int getWindowWidth();
        static int getWindowHeight();
    };
}
