#pragma once

#include "root.h"

//todo: add/remove postprocessing effects

namespace chira {
    class Frame : public Root {
    public:
        Frame(const std::string& name_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true, bool initNow = true);
        Frame(int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true, bool initNow = true);
        void render(glm::mat4 parentTransform) override;
        void render(glm::mat4 parentTransform, unsigned int parentFBOHandle, int parentWidth, int parentHeight);
        ~Frame() override;
        const Frame* getFrame() const override;
        Frame* getFrame() override;
        [[nodiscard]] unsigned int getFramebufferHandle() const;
        [[nodiscard]] unsigned int getColorTextureHandle() const;
        glm::vec<2, int> getFrameSize() const;
        virtual void setFrameSize(glm::vec<2, int> newSize);
        [[nodiscard]] ColorRGB getBackgroundColor() const;
        void setBackgroundColor(ColorRGB color);
    protected:
        unsigned int fboHandle = 0, colorTexHandle = 0, rboHandle = 0;
        ColorRGB backgroundColor{};
        int width = 0, height = 0;
        bool linearFiltering = true;
        /// Deletes the existing framebuffer if one already exists.
        void createFramebuffer();
    };
}
