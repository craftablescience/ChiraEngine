#pragma once

#include <concepts>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "Frame.h"

namespace chira {

class IPanel;

class Window : public Frame {
    // We don't want people making windows and adding them to the entity tree
    // Only the Engine can make windows!
    friend class Engine;
public:
    void render(glm::mat4 parentTransform) override;
    ~Window() override;

    unsigned int addPanel(IPanel* panel);
    [[nodiscard]] IPanel* getPanel(unsigned int panelID) const;
    template<typename T>
    requires std::derived_from<T, IPanel>
    [[nodiscard]] T* getPanel(unsigned int panelID) const {
        return assert_cast<T*>(this->getPanel(panelID));
    }
    void removePanel(unsigned int panelID);
    void removeAllPanels();

    void setFrameSize(glm::vec2i newSize) override;
    [[nodiscard]] glm::vec2d getMousePosition() const;
    void captureMouse(bool capture);
    [[nodiscard]] bool isMouseCaptured() const;
    [[nodiscard]] bool isIconified() const;
    void setVisible(bool visibility) override;
    void setFullscreen(bool goFullscreen) const;
    [[nodiscard]] bool isFullscreen() const;
    void setMaximized(bool maximize);
    [[nodiscard]] bool isMaximized() const;
    void moveToPosition(glm::vec2i pos) const;
    void moveToCenter() const;
    /// Note: Images must have a bit depth of 8
    void setIcon(const std::string& identifier) const;
    void shouldStopAfterThisFrame(bool yes = true) const;
    /// Renders the splashscreen to all window's default framebuffer
    void displaySplashScreen();
private:
    MeshDataBuilder surface;
    GLFWwindow* window = nullptr;
    ImGuiContext* imguiContext = nullptr;
    bool mouseCaptured = false, iconified = false;
    double lastMouseX = -1.0, lastMouseY = -1.0;
    std::vector<IPanel*> panels;

    Window(std::string name_, std::string_view title);
    explicit Window(std::string_view title);

    bool createGLFWWindow(std::string_view title);
    static void setImGuiConfigPath();
};

} // namespace chira
