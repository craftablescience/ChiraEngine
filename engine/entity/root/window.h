#pragma once

#include "frame.h"

namespace chira {
    class Window : public Frame {
    public:
        Window(const std::string& name_, const std::string& title_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
        Window(const std::string& title_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
        void render(glm::mat4 parentTransform) override;
        ~Window() override;
        const Window* getWindow() const override;
        Window* getWindow() override;
    };
}
