#pragma once

#include "entity3d.h"
#include "camera/camera3d.h"
#include <resource/meshResource.h>
#include <render/material/materialCubemap.h>

namespace chira {
    class Camera3d;

    class Root : private Entity3d {
        friend class Engine;
    public:
        explicit Root(Entity* parent_);
        Root(Entity* parent_, const std::string& name_);
        Root();
        explicit Root(const std::string& name_);
        void render();
        glm::vec3 getGlobalPosition() override;
        const Root* getRoot() const override;
        void setCamera(Camera3d* camera);
        Camera3d* getCamera() const;
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
        glm::vec2 getMousePosition() const;
        glm::vec2 getWindowSize() const;
        using Entity3d::hasChild;
        using Entity3d::addChild;
        using Entity3d::getChild;
        using Entity3d::removeChild;
        using Entity3d::removeAllChildren;
    private:
        SharedPointer<MeshResource> skybox;
        bool renderSkybox = false;
        Camera3d* mainCamera = nullptr;
        using Entity3d::render;
        void clearTree() const;
    };
}
