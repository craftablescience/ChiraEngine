#pragma once

#include "frame.h"

namespace chira {
    class Window : public Frame {
        // We don't want people making windows and adding them to the entity tree
        // Only the Engine can make windows!
        friend class Engine;
    public:
        void render(glm::mat4 parentTransform) override;
        ~Window() override;
        const Window* getWindow() const override;
        Window* getWindow() override;
        glm::vec2 getMousePosition() const;
    protected:
        MeshDataBuilder surface;
        Window(const std::string& name_, const std::string& title_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
        Window(const std::string& title_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
    };
}
