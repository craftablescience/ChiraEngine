#pragma once

#include "world3d.h"
#include "camera/camera3d.h"
#include <resource/meshResource.h>
#include <render/material/materialCubemap.h>

namespace chira {
    class Root : private World3d {
        friend class Engine;
    public:
        explicit Root(const std::string& name_);
        void render();
        glm::vec3 getGlobalPosition() override;
        void setMainCamera(Camera3d* camera);
        Camera3d* getMainCamera() const;
        void setSkybox(const std::string& cubemapId);
        [[nodiscard]] SharedPointer<MaterialCubemap> getSkybox() const;
        glm::vec3 getAudioListeningPosition() const {
            return this->mainCamera->getPosition();
        }
        glm::vec3 getAudioListeningRotation() const {
            return glm::eulerAngles(this->mainCamera->getRotation());
        }
        glm::vec3 getAudioListeningUpVector() const {
            return this->mainCamera->getUpVector();
        }
        using World3d::hasChild;
        using World3d::addChild;
        using World3d::getChild;
        using World3d::removeChild;
        using World3d::removeAllChildren;
    private:
        SharedPointer<MeshResource> skybox;
        bool renderSkybox = false;
        Camera3d* mainCamera = nullptr;
        using World3d::render;
        void clearTree() const;
    };
}
