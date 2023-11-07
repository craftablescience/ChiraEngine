#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

#include <entity/Viewport.h>

struct SDL_Window;
struct ImGuiContext;

namespace chira {

class IPanel;

} // namespace chira

// SDL + SDL Renderer backend
namespace chira::Device {

// Copied from SDL
enum PopupFlags {
    POPUP_ERROR                 = 0x00000010,
    POPUP_WARNING               = 0x00000020,
    POPUP_INFO                  = 0x00000040,
    POPUP_BUTTONS_LEFT_TO_RIGHT = 0x00000080,
    POPUP_BUTTONS_RIGHT_TO_LEFT = 0x00000100,
};

struct WindowHandle {
    std::unordered_map<uuids::uuid, IPanel*> panels{};
    SDL_Window* window = nullptr;
    ImGuiContext* imguiContext = nullptr;
    int width = -1;
    int height = -1;
    bool hidden = false;
    bool mouseCaptured = false;
    bool shouldClose = false;

    Viewport* viewport = nullptr;
    bool viewportIsSelfOwned = false;

    explicit inline operator bool() const { return window; }
    inline bool operator!() const { return !window; }
};

[[nodiscard]] bool initBackendAndCreateSplashscreen(bool splashScreenVisible);
void destroySplashscreen();
void destroyBackend();

[[nodiscard]] std::uint64_t getTicks();

/// Note: If an icon image is present, it must be RGBA8888
[[nodiscard]] WindowHandle* createWindow(int width, int height, std::string_view title, Viewport* viewport);
void refreshWindows();
[[nodiscard]] int getWindowCount();

[[nodiscard]] Viewport* getWindowViewport(WindowHandle* handle);

void setWindowTitle(WindowHandle* handle, std::string_view title);
[[nodiscard]] std::string_view getWindowTitle(WindowHandle* handle);

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

/// Display a popup window with the specified message.
void popup(std::string_view message, std::string_view title, unsigned int popupFlags, std::string_view ok = "OK");
/// Display a popup info window with the specified message.
void popupInfo(std::string_view message, std::string_view title = "Info");
/// Display a popup warning window with the specified message.
void popupWarning(std::string_view message, std::string_view title = "Warning");
/// Display a popup error window with the specified message.
void popupError(std::string_view message, std::string_view title = "Error");

/// Display a popup window with the specified message, as well as two buttons.
bool popupChoice(std::string_view message, std::string_view title, unsigned int popupFlags, std::string_view ok = "OK", std::string_view cancel = "Cancel");
/// Display a popup info window with the specified message, as well as Ok and Cancel buttons.
/// Returns true if Ok pressed, false if Cancel pressed.
bool popupInfoChoice(std::string_view message, std::string_view title = "Info");
/// Display a popup warning window with the specified message, as well as Ok and Cancel buttons.
/// Returns true if Ok pressed, false if Cancel pressed.
bool popupWarningChoice(std::string_view message, std::string_view title = "Warning");
/// Display a popup error window with the specified message, as well as Ok and Cancel buttons.
/// Returns true if Ok pressed, false if Cancel pressed.
bool popupErrorChoice(std::string_view message, std::string_view title = "Error");

} // namespace chira::Device
