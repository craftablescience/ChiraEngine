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
    Frame(std::string name_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true, bool initNow = true);
    void render(glm::mat4 parentTransform) override;
    void render(unsigned int parentFBOHandle, int parentWidth, int parentHeight);
    ~Frame() override;
    [[nodiscard]] glm::vec3 getGlobalPosition() override;
    [[nodiscard]] const Frame* getFrame() const override;
    [[nodiscard]] Frame* getFrame() override;
    [[nodiscard]] unsigned int getFramebufferHandle() const;
    [[nodiscard]] unsigned int getColorTextureHandle() const;
    virtual void setFrameSize(glm::vec2i newSize);
    [[nodiscard]] glm::vec2i getFrameSize() const;
    [[nodiscard]] ColorRGB getBackgroundColor() const;
    void setBackgroundColor(ColorRGB color);
    void setCamera(Camera* camera);
    [[nodiscard]] Camera* getCamera() const;
    void setSkybox(const std::string& cubemapId);
    [[nodiscard]] SharedPointer<MaterialCubemap> getSkybox() const;
    [[nodiscard]] LightManager* getLightManager();
protected:
    unsigned int fboHandle = 0, colorTexHandle = 0, rboHandle = 0;
    ColorRGB backgroundColor{};
    int width = 0, height = 0;
    bool linearFiltering = true;

    MeshDataBuilder skybox;
    bool renderSkybox = false;
    bool skyboxMeshCreated = false;
    Camera* mainCamera = nullptr;

    LightManager lightManager{};

    /// Deletes the existing framebuffer if one already exists.
    void createFramebuffer();
};

} // namespace chira
