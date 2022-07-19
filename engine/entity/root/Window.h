#pragma once

#include "Frame.h"
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
    [[nodiscard]] const Window* getWindow() const override;
    [[nodiscard]] Window* getWindow() override;
    void setFrameSize(glm::vec2i newSize) override;
    [[nodiscard]] glm::vec2d getMousePosition() const;
    void captureMouse(bool capture);
    [[nodiscard]] bool isMouseCaptured() const;
    [[nodiscard]] bool isIconified() const;
    void setVisible(bool visibility) override;
    /// Note: Images must have a bit depth of 8
    void setIcon(const std::string& identifier) const;
    void shouldStopAfterThisFrame(bool yes = true) const;
    /// Renders the splashscreen to all window's default framebuffer
    void displaySplashScreen();
protected:
    MeshDataBuilder surface;
    GLFWwindow* window = nullptr;
    bool mouseCaptured = false, iconified = false, fullscreen, vsync;
    double lastMouseX = -1.0, lastMouseY = -1.0;
    Window(std::string name_, std::string_view title, int width_, int height_, bool fullscreen_ = false, bool vsync_ = true, ColorRGB backgroundColor_ = {}, bool smoothResize = true, bool startVisible = false);
    Window(std::string_view title, int width_, int height_, bool fullscreen_ = false, bool vsync_ = true, ColorRGB backgroundColor_ = {}, bool smoothResize = true, bool startVisible = false);
private:
    bool createGLFWWindow(std::string_view title);
};

}
