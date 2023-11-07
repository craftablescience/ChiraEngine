#pragma once

#include <render/backend/RenderBackend.h>
#include "Scene.h"

namespace chira {

class Viewport {
public:
    explicit Viewport(glm::vec2i size_, ColorRGB backgroundColor_ = {}, bool linearFiltering_ = true);

    Scene* addScene();

    Scene* addScene(const std::string& name);

    Scene* addScene(uuids::uuid uuid);

    Scene* addScene(uuids::uuid uuid, const std::string& name);

    [[nodiscard]] Scene* getScene(uuids::uuid sceneID);

    [[nodiscard]] const auto& getScenes() const {
        return this->scenes;
    }

    [[nodiscard]] bool hasScene(uuids::uuid sceneID);

    void removeScene(uuids::uuid sceneID);

    void removeAllScenes();

    void update();

    void render();

    [[nodiscard]] CameraComponent* getCamera() const {
        for (const auto& [uuid, scene] : this->scenes) {
            if (auto* camera = scene->getCamera()) {
                return camera;
            }
        }
        return nullptr;
    }

    [[nodiscard]] ColorRGB getBackgroundColor() const {
        return this->backgroundColor;
    }

    void setBackgroundColor(ColorRGB color) {
        this->backgroundColor = color;
    }

    [[nodiscard]] glm::vec2i getSize() const {
        return this->size;
    }

    void setSize(glm::vec2i size_) {
        this->size = size_;
        this->recreateFrameBuffer();
    }

    [[nodiscard]] bool isLinearFiltered() const {
        return this->linearFiltering;
    }

    void setLinearFiltering(bool enable) {
        this->linearFiltering = enable;
        this->recreateFrameBuffer();
    }

    [[nodiscard]] Renderer::FrameBufferHandle* getRawHandle() {
        return &this->frameBufferHandle;
    }

private:
    void recreateFrameBuffer();

private:
    std::unordered_map<uuids::uuid, std::unique_ptr<Scene>> scenes;
    Renderer::FrameBufferHandle frameBufferHandle;
    glm::vec2i size;
    ColorRGB backgroundColor;
    bool linearFiltering;
};

} // namespace chira
