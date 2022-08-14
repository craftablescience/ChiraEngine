#pragma once

#include <entity/camera/Camera.h>
#include <render/mesh/MeshDataBuilder.h>
#include <render/material/MaterialCubemap.h>
#include <math/Types.h>
#include "Group.h"

//todo: add/remove postprocessing effects

namespace chira {

class Frame : public Group {
public:
    Frame(std::string name_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true, bool initNow = true);
    Frame(int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true, bool initNow = true);
    void render(glm::mat4 parentTransform) override;
    void render(unsigned int parentFBOHandle, int parentWidth, int parentHeight, Camera* camera);
    ~Frame() override;
    [[nodiscard]] const Frame* getFrame() const override;
    [[nodiscard]] Frame* getFrame() override;
    [[nodiscard]] Camera* getCamera() const;
    void setCamera(Camera* camera);
    [[nodiscard]] unsigned int getFramebufferHandle() const;
    [[nodiscard]] unsigned int getColorTextureHandle() const;
    [[nodiscard]] glm::vec2i getFrameSize() const;
    virtual void setFrameSize(glm::vec2i newSize);
    [[nodiscard]] ColorRGB getBackgroundColor() const;
    void setBackgroundColor(ColorRGB color);
    [[nodiscard]] SharedPointer<MaterialCubemap> getSkybox() const;
    void setSkybox(const std::string& cubemapId);
protected:
    MeshDataBuilder skybox;
    bool renderSkybox = false;
    bool skyboxMeshCreated = false;
    Camera* mainCamera = nullptr;
    unsigned int fboHandle = 0, colorTexHandle = 0, rboHandle = 0;
    ColorRGB backgroundColor{};
    int width = 0, height = 0;
    bool linearFiltering = true;
    /// Deletes the existing framebuffer if one already exists.
    void createFramebuffer();
};

} // namespace chira
