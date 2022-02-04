#pragma once

#include <entity/entity.h>
#include <entity/camera/camera.h>
#include <render/mesh/meshDataBuilder.h>
#include <render/material/materialCubemap.h>

namespace chira {
    class Camera;

    /// The Root is an in-world entity. It is designed to contain a collection of entities, like a "map".
    /// It will never be the true root of the scene, that honor goes to a FrameRoot or WindowRoot.
    class Root : public Entity {
        friend class Engine;
    public:
        explicit Root(const std::string& name_);
        Root();
        void render(glm::mat4 parentTransform) override;
        glm::vec3 getGlobalPosition() override;
        const Root* getRoot() const override;
        Root* getRoot() override;
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
        virtual glm::vec2 getMousePosition() const;
        virtual glm::vec2 getFrameSize() const;
    protected:
        MeshDataBuilder skybox;
        bool renderSkybox = false;
        Camera* mainCamera = nullptr;
        void clearTree() const;
    };
}
