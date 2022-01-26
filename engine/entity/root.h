#pragma once

#include <entity/entity.h>
#include <entity/camera/camera.h>
#include <render/mesh/meshDataBuilder.h>
#include <render/material/materialCubemap.h>

namespace chira {
    class Camera;

    class Root : private Entity {
        friend class Engine;
    public:
        explicit Root(Entity* parent_);
        Root(Entity* parent_, const std::string& name_);
        Root();
        explicit Root(const std::string& name_);
        void render();
        glm::vec3 getGlobalPosition() override;
        const Root* getRoot() const override;
        void setCamera(Camera* camera);
        Camera* getCamera() const;
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
        using Entity::hasChild;
        using Entity::addChild;
        using Entity::getChild;
        using Entity::removeChild;
        using Entity::removeAllChildren;
    private:
        using Entity::render;
        MeshDataBuilder skybox;
        bool renderSkybox = false;
        Camera* mainCamera = nullptr;
        void clearTree() const;
    };
}
