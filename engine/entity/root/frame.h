#pragma once

#include "root.h"

namespace chira {
    class Frame : public Root {
    public:
        Frame(const std::string& name_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
        Frame(int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
        void render(glm::mat4 parentTransform) override;
        ~Frame() override;
        const Frame* getFrame() const override;
        Frame* getFrame() override;
        [[nodiscard]] unsigned int getFramebufferHandle() const;
        [[nodiscard]] unsigned int getColorTextureHandle() const;
        glm::vec2 getFrameSize() const override;
        [[nodiscard]] ColorRGB getBackgroundColor() const;
        void setBackgroundColor(ColorRGB color);
    protected:
        unsigned int fboHandle = 0, colorTexHandle = 0, rboHandle = 0;
        ColorRGB backgroundColor{};
        int width = 0, height = 0;
        /// Only run this once on construction please. Thanks
        void createFramebuffer(bool smoothResize);
    };
}
