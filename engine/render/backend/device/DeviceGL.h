#pragma once

#include <unordered_map>
#include <entity/root/Frame.h>
#include <utility/UUIDGenerator.h>

struct SDL_Window;
struct ImGuiContext;

namespace chira {

class IPanel;

class Device {
    friend class Engine;
private:
    explicit Device(std::string_view title);
public:
    void refresh();
    ~Device();

    [[nodiscard]] Frame* getFrame();

    uuids::uuid addPanel(IPanel* panel);
    [[nodiscard]] IPanel* getPanel(const uuids::uuid& panelID);
    void removePanel(const uuids::uuid& panelID);
    void removeAllPanels();

    void setSize(glm::vec2i newSize, bool setWindowSize = true);
    [[nodiscard]] static glm::vec2i getMousePosition();
    void captureMouse(bool capture);
    [[nodiscard]] bool isMouseCaptured() const;
    [[nodiscard]] bool isIconified() const;
    void setVisible(bool visibility);
    void setFullscreen(bool goFullscreen) const;
    [[nodiscard]] bool isFullscreen() const;
    void setMaximized(bool maximize);
    [[nodiscard]] bool isMaximized() const;
    void moveToPosition(glm::vec2i pos) const;
    void moveToCenter() const;

    /// Note: Images must have a bit depth of 8
    void setIcon(const std::string& identifier) const;

    [[nodiscard]] bool shouldCloseAfterThisFrame() const;
    void closeAfterThisFrame(bool yes = true);

    /// Renders the splashscreen to the window's default framebuffer
    void displaySplashScreen();

private:
    Frame frame;
    bool visible = true;
    MeshDataBuilder surface{};
    SDL_Window* window = nullptr;
    void* glContext = nullptr;
    ImGuiContext* imguiContext = nullptr;
    bool mouseCaptured = false, iconified = false, shouldClose = false;
    int width = -1, height = -1;
    std::unordered_map<uuids::uuid, IPanel*> panels{};

    bool createGLFWWindow(std::string_view title);
};

} // namespace chira
