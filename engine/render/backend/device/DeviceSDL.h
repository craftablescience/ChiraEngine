#pragma once

#include <unordered_map>
#include <entity/root/Frame.h>
#include <utility/UUIDGenerator.h>

struct SDL_Window;
struct ImGuiContext;

namespace chira {

class IPanel;

struct Window : public Frame {
    friend class Engine;
public:
    void render(glm::mat4) override;
    ~Window() override;
    uuids::uuid addPanel(IPanel* panel);
    [[nodiscard]] IPanel* getPanel(const uuids::uuid& panelID);
    void removePanel(const uuids::uuid& panelID);
    void removeAllPanels();
    void setSize(glm::vec2i newSize, bool setWindowSize = true);
    [[nodiscard]] static glm::vec2i getMousePosition();
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
    void shouldCloseAfterThisFrame(bool yes = true);
    /// Renders the splashscreen to all window's default framebuffer
    void displaySplashScreen();
private:
    MeshDataBuilder surface;
    SDL_Window* window = nullptr;
    void* glContext = nullptr;
    ImGuiContext* imguiContext = nullptr;
    bool mouseCaptured = false, iconified = false, shouldClose = false;
    double lastMouseX = -1.0, lastMouseY = -1.0;
    std::unordered_map<uuids::uuid, IPanel*> panels{};

    Window(std::string name_, std::string_view title);
    explicit Window(std::string_view title);

    bool createGLFWWindow(std::string_view title);
    static void setImGuiConfigPath();
};

} // namespace chira
