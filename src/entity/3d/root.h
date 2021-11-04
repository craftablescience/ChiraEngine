#pragma once

#include "world3d.h"
#include "../../resource/meshResource.h"
#include "../../render/cubemapMaterial.h"

namespace chira {
    class root : private world3d {
    public:
        explicit root(const std::string& name_);
        // void setCamera();
        // camera* getCamera();
        void render();
        void setSkybox(const std::string& cubemapId);
        cubemapMaterial* getSkybox();
        glm::vec3 getAudioListeningPosition() {
            return glm::vec3{}; // todo
        }
        glm::vec3 getAudioListeningRotation() {
            return glm::vec3{}; // todo
        }
        glm::vec3 getAudioListeningUpVector() {
            return glm::vec3{}; // todo
        }
        using world3d::addChild;
        using world3d::getChild;
        using world3d::removeChild;
    private:
        meshResource* skybox;
        using world3d::render;
    };
}
