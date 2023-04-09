#pragma once

#include <entity/camera/Camera.h>
#include <math/Types.h>
#include <render/mesh/MeshDataBuilder.h>
#include <render/material/MaterialCubemap.h>

#include "Group.h"
#include "../light/LightManager.h"

namespace chira {

class Frame : public Group {
public:
    Frame(std::string name_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
    Frame(int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
    /// Deletes and recreates the existing framebuffer if one already exists.
    /// If one doesn't exist, initialize a new framebuffer.
    void recreateFramebuffer();
    void render(glm::mat4 parentTransform) override;
    ~Frame() override;
    void useFrameBufferTexture(TextureUnit activeTextureUnit = TextureUnit::G0) const;
    [[nodiscard]] glm::vec3 getGlobalPosition() override;
    [[nodiscard]] const Frame* getFrame() const override;
    [[nodiscard]] Frame* getFrame() override;
    virtual void setFrameSize(glm::vec2i newSize);
    [[nodiscard]] glm::vec2i getFrameSize() const;
    [[nodiscard]] ColorRGB getBackgroundColor() const;
    void setBackgroundColor(ColorRGB color);
    void setCamera(Camera* camera);
    [[nodiscard]] Camera* getCamera() const;
    void setSkybox(const std::string& cubemapId);
    [[nodiscard]] SharedPointer<MaterialCubemap> getSkybox() const;
    [[nodiscard]] LightManager* getLightManager();
    [[nodiscard]] Renderer::FrameBufferHandle* getRawHandle();
protected:
    ColorRGB backgroundColor{};
    Renderer::FrameBufferHandle handle{};
    int width = 0, height = 0;
    bool linearFiltering = true;

    MeshDataBuilder skybox;
    bool renderSkybox = false;
    bool skyboxMeshCreated = false;
    Camera* mainCamera = nullptr;

    LightManager lightManager{};
};

} // namespace chira
