#pragma once

#include "frame.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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
        void setFrameSize(glm::vec<2, int> newSize) override;
        glm::vec<2, double> getMousePosition() const;
        void captureMouse(bool capture);
        [[nodiscard]] bool isMouseCaptured() const;
        [[nodiscard]] bool isIconified() const;
        /// Note: Images must have a bit depth of 8
        void setIcon(const std::string& identifier) const;
        void shouldStopAfterThisFrame(bool yes = true) const;
        /// Renders the splashscreen to all window's default framebuffer
        static void displaySplashScreen();
    protected:
        MeshDataBuilder surface;
        GLFWwindow* window = nullptr;
        bool mouseCaptured = false, iconified = false;
        double lastMouseX = -1.0, lastMouseY = -1.0;
        Window(const std::string& name_, const std::string& title_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
        Window(const std::string& title_, int width_, int height_, ColorRGB backgroundColor_ = {}, bool smoothResize = true);
    private:
        bool createGLFWWindow();
    };
}
