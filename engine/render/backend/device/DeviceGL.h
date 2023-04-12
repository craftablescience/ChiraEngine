#pragma once

#include <unordered_map>
#include <entity/Layer.h>

struct SDL_Window;
struct ImGuiContext;

namespace chira {

class IPanel;

} // namespace chira

// OpenGL 4.1 (or 4.3) SDL backend
namespace chira::Device {

struct WindowHandle {
    std::unordered_map<uuids::uuid, IPanel*> panels{};
    SDL_Window* window = nullptr;
    ImGuiContext* imguiContext = nullptr;
    int width = -1;
    int height = -1;
    bool hidden = false;
    bool mouseCaptured = false;
    bool shouldClose = false;

    Layer* layer = nullptr;
    bool layerIsSelfOwned = false;

    explicit inline operator bool() const { return window; }
    inline bool operator!() const { return !window; }
};

[[nodiscard]] bool initBackendAndCreateSplashscreen(bool splashScreenVisible);
void destroySplashscreen();
void destroyBackend();

/// Note: If an icon image is present, it must be RGBA8888
[[nodiscard]] WindowHandle* createWindow(int width, int height, std::string_view title, Layer* layer);
void refreshWindows();
[[nodiscard]] int getWindowCount();

[[nodiscard]] Layer* getWindowLayer(WindowHandle* handle);

void setWindowMaximized(WindowHandle* handle, bool maximize);
[[nodiscard]] bool isWindowMaximized(WindowHandle* handle);

void minimizeWindow(WindowHandle* handle, bool minimize);
[[nodiscard]] bool isWindowMinimized(WindowHandle* handle);

void setWindowFullscreen(WindowHandle* handle, bool fullscreen);
[[nodiscard]] bool isWindowFullscreen(WindowHandle* handle);

void setWindowVisibility(WindowHandle* handle, bool visible);
[[nodiscard]] bool isWindowVisible(WindowHandle* handle);

void setWindowSize(WindowHandle* handle, int width, int height);
[[nodiscard]] glm::vec2i getWindowSize(WindowHandle* handle);

void setWindowPosition(WindowHandle* handle, int width, int height);
void setWindowPositionFromCenter(WindowHandle* handle, int width, int height);
[[nodiscard]] glm::vec2i getWindowPosition(WindowHandle* handle);

void setMousePositionGlobal(int x, int y);
void setMousePositionInWindow(WindowHandle* handle, int x, int y);
[[nodiscard]] glm::vec2i getMousePositionGlobal();
[[nodiscard]] glm::vec2i getMousePositionInFocusedWindow();

void setMouseCapturedWindow(WindowHandle* handle, bool captured);
[[nodiscard]] bool isMouseCapturedWindow(WindowHandle* handle);

/// Destroys windows the next time refreshWindows() is called
void queueDestroyWindow(WindowHandle* handle, bool free);
[[nodiscard]] bool isWindowAboutToBeDestroyed(WindowHandle* handle);
void destroyWindow(WindowHandle* handle);
void destroyAllWindows();

uuids::uuid addPanelToWindow(WindowHandle* handle, IPanel* panel);
[[nodiscard]] IPanel* getPanelOnWindow(WindowHandle* handle, const uuids::uuid& panelID);
void removePanelFromWindow(WindowHandle* handle, const uuids::uuid& panelID);
void removeAllPanelsFromWindow(WindowHandle* handle);

} // namespace Renderer
