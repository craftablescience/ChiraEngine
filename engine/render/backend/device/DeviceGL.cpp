#include "DeviceGL.h"

#include <array>

#include <backends/imgui_impl_sdl2.h>
#include <glad/gl.h>
#include <glad/glversion.h>
#include <imgui.h>
#include <SDL.h>

#include <config/Config.h>
#include <config/ConEntry.h>
#include <event/Events.h>
#include <input/InputManager.h>
#include <loader/image/Image.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <render/material/MaterialFrameBuffer.h>
#include <render/material/MaterialTextured.h>
#include <render/mesh/MeshDataBuilder.h>
#include <ui/Font.h>
#include <ui/IPanel.h>

using namespace chira;

CHIRA_CREATE_LOG(WINDOW);

static void setVSync(bool enable) {
    if (!enable) {
        SDL_GL_SetSwapInterval(0);
    } else if (SDL_GL_SetSwapInterval(-1) == -1) {
        // Fall back to regular vsync if we don't have adaptive vsync
        SDL_GL_SetSwapInterval(1);
    }
}

ConVar win_vsync{"win_vsync", true, "Limit the FPS to your monitor's resolution.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    setVSync(static_cast<bool>(std::stoi(newValue.data())));
}};

[[maybe_unused]]
ConVar input_raw_mouse_motion{"input_raw_mouse_motion", true, "Get more accurate mouse motion.", CON_FLAG_CACHE};

static void setImGuiConfigPath() {
    static std::string configPath = Config::getConfigFile("imgui.ini");
    ImGui::GetIO().IniFilename = configPath.c_str();
}

Renderer::FrameBufferHandle g_WindowFramebufferHandle{};
SDL_Window* g_Splashscreen = nullptr;
void* g_GLContext = nullptr;

bool Device::initBackendAndCreateSplashscreen(bool splashScreenVisible) {
    static bool alreadyRan = false;
    if (alreadyRan)
        return false;
    alreadyRan = true;

    int windowFlags =
            SDL_WINDOW_OPENGL |
            SDL_WINDOW_ALLOW_HIGHDPI |
            SDL_WINDOW_ALWAYS_ON_TOP |
            SDL_WINDOW_BORDERLESS |
            (splashScreenVisible ? SDL_WINDOW_SHOWN : SDL_WINDOW_HIDDEN);
    int width = 640, height = 480;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    g_Splashscreen = SDL_CreateWindow("Loading...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
    if (!g_Splashscreen) {
        LOG_WINDOW.error("Splashscreen window creation failed! Error: {}", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_VERSION_MINOR);

    int glContextFlags;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &glContextFlags);
    glContextFlags |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#ifdef DEBUG
    glContextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glContextFlags);

    g_GLContext = SDL_GL_CreateContext(g_Splashscreen);
    if (!g_GLContext) {
        LOG_WINDOW.error("Splashscreen window context creation failed! Error: {}", SDL_GetError());
        return false;
    }
    if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {
        LOG_WINDOW.error("{} must be available to run this program!", GL_VERSION_STRING_PRETTY);
        return false;
    }
    if (SDL_GL_MakeCurrent(g_Splashscreen, g_GLContext)) {
        LOG_WINDOW.error("Splashscreen window context failed to be made current! Error: {}", SDL_GetError());
        return false;
    }
    setVSync(win_vsync.getValue<bool>());

    Renderer::pushFrameBuffer(g_WindowFramebufferHandle);
    Renderer::recreateFrameBuffer(&g_WindowFramebufferHandle, width, height, WrapMode::REPEAT, WrapMode::REPEAT, FilterMode::LINEAR, true);
    glViewport(0, 0, width, height);

    MeshDataBuilder plane;
    plane.addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    plane.setMaterial(Resource::getResource<MaterialTextured>("file://materials/splashscreen.json").castAssert<IMaterial>());
    plane.render(glm::identity<glm::mat4>());

    glDisable(GL_DEPTH_TEST);
    Renderer::popFrameBuffer();

    MeshDataBuilder windowSurface;
    windowSurface.addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    windowSurface.setMaterial(Resource::getResource<MaterialFrameBuffer>("file://materials/window.json", &g_WindowFramebufferHandle).castAssert<IMaterial>());
    windowSurface.render(glm::identity<glm::mat4>());

    glEnable(GL_DEPTH_TEST);
    SDL_GL_SwapWindow(g_Splashscreen);

    return true;
}

void Device::destroySplashscreen() {
    if (g_Splashscreen) {
        SDL_DestroyWindow(g_Splashscreen);
        g_Splashscreen = nullptr;
    }
}

void Device::destroyBackend() {
    Renderer::destroyImGui();
    Device::destroyAllWindows();
    SDL_GL_DeleteContext(g_GLContext);
}

std::array<Device::WindowHandle, 256> WINDOWS{};

static int findFreeWindow() {
    for (int i = 0; i < WINDOWS.size(); i++) {
        if (!WINDOWS.at(i))
            return i;
    }
    return -1;
}

[[nodiscard]] Device::WindowHandle* Device::createWindow(int width, int height, std::string_view title, Layer* layer) {
    int freeWindow = findFreeWindow();
    if (freeWindow == -1)
        return nullptr;

    WINDOWS[freeWindow] = WindowHandle{};
    WindowHandle& handle = WINDOWS.at(freeWindow);

    handle.width = width;
    handle.height = height;

    int windowFlags =
            SDL_WINDOW_OPENGL |
            SDL_WINDOW_ALLOW_HIGHDPI |
            SDL_WINDOW_RESIZABLE |
            SDL_WINDOW_SHOWN;
    handle.window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
    if (!handle.window) {
        LOG_WINDOW.error("Window creation failed! Error: {}", SDL_GetError());
        return nullptr;
    }
    SDL_SetWindowData(handle.window, "handle", &handle);
    SDL_GL_MakeCurrent(handle.window, g_GLContext);

    if (layer) {
        handle.layer = layer;
        handle.layerIsSelfOwned = false;
    } else {
        handle.layer = new Layer{{width, height}};
        handle.layerIsSelfOwned = true;
    }

    int iconWidth, iconHeight, bitsPerPixel;
    auto* icon = Image::getUncompressedImage(FilesystemResourceProvider::getResourceAbsolutePath("file://textures/ui/icon.png"), &iconWidth, &iconHeight, &bitsPerPixel, 4, false);
    if (icon) {
        auto* sdlIcon = SDL_CreateRGBSurfaceWithFormatFrom(icon, iconWidth, iconHeight, bitsPerPixel, 8, SDL_PIXELFORMAT_RGBA8888);
        SDL_SetWindowIcon(handle.window, sdlIcon);
        SDL_FreeSurface(sdlIcon);
        Image::deleteUncompressedImage(icon);
    }

    handle.imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(handle.imguiContext);
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_DockingEnable;
    setImGuiConfigPath();

    Renderer::initImGui(handle.window, g_GLContext);

    auto defaultFont = Resource::getUniqueUncachedResource<Font>("file://fonts/default.json");
    io.FontDefault = defaultFont->getFont();
    io.Fonts->Build();

    return &handle;
}

void Device::refreshWindows() {
    // Render each window
    for (auto& handle : WINDOWS) {
        if (!handle)
            continue;

        if (Device::isWindowAboutToBeDestroyed(&handle)) {
            Device::destroyWindow(&handle);
            continue;
        }

        if (!Device::isWindowVisible(&handle)) {
            handle.layer->update();
            continue;
        }

        SDL_GL_MakeCurrent(handle.window, g_GLContext);
        glViewport(0, 0, handle.width, handle.height);

        ImGui::SetCurrentContext(handle.imguiContext);
        setImGuiConfigPath();

        Renderer::pushFrameBuffer(*handle.layer->getRawHandle());
        Renderer::startImGuiFrame();

        handle.layer->update();
        handle.layer->render();

        for (auto& [uuid, panel] : handle.panels) {
            panel->render();
        }

        glDisable(GL_DEPTH_TEST);

        Renderer::endImGuiFrame();
        Renderer::popFrameBuffer();

        MeshDataBuilder surface;
        surface.addSquare({}, {2, -2}, SignedAxis::ZN, 0);
        surface.setMaterial(Resource::getUniqueUncachedResource<MaterialFrameBuffer>("file://materials/window.json", handle.layer->getRawHandle()).castAssert<IMaterial>());
        surface.render(glm::identity<glm::mat4>());

        glEnable(GL_DEPTH_TEST);

        SDL_GL_SwapWindow(handle.window);
    }

    // Process input
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        auto* handle = reinterpret_cast<Device::WindowHandle*>(SDL_GetWindowData(SDL_GetWindowFromID(event.window.windowID), "handle"));
        if (!handle)
            continue;

        ImGui::SetCurrentContext(handle->imguiContext);
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                for (auto& windowHandle : WINDOWS) {
                    if (windowHandle) {
                        Device::queueDestroyWindow(&windowHandle, true);
                    }
                }
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        Device::queueDestroyWindow(handle, true);
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    case SDL_WINDOWEVENT_MAXIMIZED:
                        SDL_GetWindowSizeInPixels(handle->window, &handle->width, &handle->height);
                        handle->layer->setSize({handle->width, handle->height});
                        break;
                    default:
                        // There's quite a few events we don't care about
                        break;
                }
                break;
            case SDL_KEYDOWN:
                for (const auto& keyEvent : Input::KeyEvent::getEvents()) {
                    if (keyEvent.getEvent() == event.key.keysym.sym && keyEvent.getEventType() == Input::KeyEventType::PRESSED) {
                        keyEvent();
                    }
                }
                break;
            case SDL_KEYUP:
                for (const auto& keyEvent : Input::KeyEvent::getEvents()) {
                    if (keyEvent.getEvent() == event.key.keysym.sym && keyEvent.getEventType() == Input::KeyEventType::RELEASED) {
                        keyEvent();
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                for (const auto& mouseEvent : Input::MouseEvent::getEvents()) {
                    if (static_cast<uint8_t>(mouseEvent.getEvent()) == event.button.button && mouseEvent.getEventType() == Input::MouseEventType::CLICKED) {
                        mouseEvent(event.button.x, event.button.y, event.button.clicks);
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                for (const auto& mouseEvent : Input::MouseEvent::getEvents()) {
                    if (static_cast<uint8_t>(mouseEvent.getEvent()) == event.button.button && mouseEvent.getEventType() == Input::MouseEventType::RELEASED) {
                        mouseEvent(event.button.x, event.button.y, event.button.clicks);
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                for (const auto& mouseMotionEvent : Input::MouseMotionEvent::getEvents()) {
                    if (mouseMotionEvent.getEvent() == Input::MouseMotion::MOVEMENT) {
                        mouseMotionEvent(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
                    }
                }
                break;
            case SDL_MOUSEWHEEL:
                for (const auto& mouseMotionEvent : Input::MouseMotionEvent::getEvents()) {
                    if (mouseMotionEvent.getEvent() == Input::MouseMotion::SCROLL) {
                        mouseMotionEvent(event.wheel.x, event.wheel.y, event.wheel.x, event.wheel.y);
                    }
                }
                break;
            default:
                // todo(input): handle joystick / game controller inputs!
                break;
        }
    }

    // Handle repeating events
    // This is a pointer to a static variable in SDL so this is safe
    static const auto* keyStates = SDL_GetKeyboardState(nullptr);
    for (const auto& keyEvent : Input::KeyEvent::getEvents()) {
        if (keyStates[SDL_GetScancodeFromKey(keyEvent.getEvent())] && keyEvent.getEventType() == Input::KeyEventType::REPEATED) {
            keyEvent();
        }
    }
}

[[nodiscard]] int Device::getWindowCount() {
    int count = 0;
    for (const auto& handle : WINDOWS) {
        count += static_cast<bool>(handle);
    }
    return count;
}

[[nodiscard]] Layer* Device::getWindowLayer(WindowHandle* handle) {
    return handle->layer;
}

void Device::setWindowMaximized(WindowHandle* handle, bool maximize) {
    if (Device::isWindowFullscreen(handle))
        return;
    if (maximize) {
        SDL_MaximizeWindow(handle->window);
    } else {
        SDL_RestoreWindow(handle->window);
    }
    SDL_GetWindowSize(handle->window, &handle->width, &handle->height);
    handle->layer->setSize({handle->width, handle->height});
}

[[nodiscard]] bool Device::isWindowMaximized(WindowHandle* handle) {
    return SDL_GetWindowFlags(handle->window) & SDL_WINDOW_MAXIMIZED;
}

void Device::minimizeWindow(WindowHandle* handle, bool minimize) {
    if (minimize) {
        SDL_MinimizeWindow(handle->window);
    } else {
        SDL_RestoreWindow(handle->window);
    }
}

[[nodiscard]] bool Device::isWindowMinimized(WindowHandle* handle) {
    return SDL_GetWindowFlags(handle->window) & SDL_WINDOW_MINIMIZED;
}

void Device::setWindowFullscreen(WindowHandle* handle, bool fullscreen) {
    SDL_SetWindowFullscreen(handle->window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

[[nodiscard]] bool Device::isWindowFullscreen(WindowHandle* handle) {
    return SDL_GetWindowFlags(handle->window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
}

void Device::setWindowVisibility(WindowHandle* handle, bool visible) {
    if (visible) {
        SDL_ShowWindow(handle->window);
    } else {
        SDL_HideWindow(handle->window);
    }
    handle->hidden = visible;
}

[[nodiscard]] bool Device::isWindowVisible(WindowHandle* handle) {
    return !handle->hidden;
}

void Device::setWindowSize(WindowHandle* handle, int width, int height) {
    handle->width = width;
    handle->height = height;
    handle->layer->setSize({width, height});
    SDL_SetWindowSize(handle->window, width, height);
}

[[nodiscard]] glm::vec2i Device::getWindowSize(WindowHandle* handle) {
    glm::vec2i dims;
    SDL_GetWindowSize(handle->window, &dims.x, &dims.y);
    return dims;
}

void Device::setWindowPosition(WindowHandle* handle, int width, int height) {
    if (Device::isWindowFullscreen(handle))
        return;
    SDL_SetWindowPosition(handle->window, width, height);
}

void Device::setWindowPositionFromCenter(WindowHandle* handle, int width, int height) {
    if (Device::isWindowFullscreen(handle))
        return;
    SDL_Rect rect;
    SDL_GetDisplayBounds(0, &rect);
    SDL_SetWindowPosition(handle->window, (rect.w / 2) + width, (rect.h / 2) + height);
}

[[nodiscard]] glm::vec2i Device::getWindowPosition(WindowHandle* handle) {
    glm::vec2i pos;
    SDL_GetWindowPosition(handle->window, &pos.x, &pos.y);
    return pos;
}

void Device::setMousePositionGlobal(int x, int y) {
    SDL_WarpMouseGlobal(x, y);
}

void Device::setMousePositionInWindow(WindowHandle* handle, int x, int y) {
    SDL_WarpMouseInWindow(handle->window, x, y);
}

[[nodiscard]] glm::vec2i Device::getMousePositionGlobal() {
    glm::vec2i pos{-1, -1};
    SDL_GetGlobalMouseState(&pos.x, &pos.y);
    return pos;
}

[[nodiscard]] glm::vec2i Device::getMousePositionInFocusedWindow() {
    glm::vec2i pos{-1, -1};
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

void Device::setMouseCapturedWindow(WindowHandle* handle, bool captured) {
    SDL_RaiseWindow(handle->window);
    ImGui::SetCurrentContext(handle->imguiContext);
    setImGuiConfigPath();

    if (captured) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    } else {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
    handle->mouseCaptured = captured;
}

[[nodiscard]] bool Device::isMouseCapturedWindow(WindowHandle* handle) {
    return handle->mouseCaptured;
}

/// Destroys windows the next time refreshWindows() is called
void Device::queueDestroyWindow(WindowHandle* handle, bool free) {
    handle->shouldClose = free;
}

[[nodiscard]] bool Device::isWindowAboutToBeDestroyed(WindowHandle* handle) {
    return handle->shouldClose;
}

void Device::destroyWindow(WindowHandle* handle) {
    if (handle->layerIsSelfOwned) {
        delete handle->layer;
    }
    Device::removeAllPanelsFromWindow(handle);
    ImGui::DestroyContext(handle->imguiContext);
    handle->imguiContext = nullptr;
    SDL_DestroyWindow(handle->window);
    handle->window = nullptr;
}

void Device::destroyAllWindows() {
    for (auto& handle : WINDOWS) {
        if (handle) {
            Device::destroyWindow(&handle);
        }
    }
}

uuids::uuid Device::addPanelToWindow(WindowHandle* handle, IPanel* panel) {
    const auto uuid = UUIDGenerator::getNewUUID();
    handle->panels[uuid] = panel;
    return uuid;
}

[[nodiscard]] IPanel* Device::getPanelOnWindow(WindowHandle* handle, const uuids::uuid& panelID) {
    if (handle->panels.contains(panelID))
        return handle->panels[panelID];
    return nullptr;
}

void Device::removePanelFromWindow(WindowHandle* handle, const uuids::uuid& panelID) {
    if (handle->panels.contains(panelID)) {
        delete handle->panels[panelID];
        handle->panels.erase(panelID);
    }
}

void Device::removeAllPanelsFromWindow(WindowHandle* handle) {
    for (const auto& [panelID, panel] : handle->panels) {
        delete panel;
    }
    handle->panels.clear();
}

void Device::popup(std::string_view message, std::string_view title, unsigned int popupFlags, std::string_view ok) {
    SDL_MessageBoxButtonData buttons[] {
            {
                    .flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
                    .buttonid = 0,
                    .text = ok.data(),
            },
    };
    SDL_MessageBoxData data{
            .flags = popupFlags,
            .window = nullptr,
            .title = title.data(),
            .message = message.data(),
            .numbuttons = 1,
            .buttons = buttons,
            .colorScheme = nullptr,
    };
    int buttonID;
    SDL_ShowMessageBox(&data, &buttonID);
}

void Device::popupInfo(std::string_view message, std::string_view title) {
    Device::popup(message, title, POPUP_INFO);
}

void Device::popupWarning(std::string_view message, std::string_view title) {
    Device::popup(message, title, POPUP_WARNING);
}

void Device::popupError(std::string_view message, std::string_view title) {
    Device::popup(message, title, POPUP_ERROR);
}

bool Device::popupChoice(std::string_view message, std::string_view title, unsigned int popupFlags, std::string_view ok, std::string_view cancel) {
    SDL_MessageBoxButtonData buttons[] {
            {
                    .flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
                    .buttonid = 1,
                    .text = ok.data(),
            },
            {
                    .flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,
                    .buttonid = 0,
                    .text = cancel.data(),
            }
    };
    SDL_MessageBoxData data{
            .flags = popupFlags,
            .window = nullptr,
            .title = title.data(),
            .message = message.data(),
            .numbuttons = 2,
            .buttons = buttons,
            .colorScheme = nullptr,
    };
    int buttonID;
    SDL_ShowMessageBox(&data, &buttonID);
    return buttonID;
}

bool Device::popupInfoChoice(std::string_view message, std::string_view title) {
    return Device::popupChoice(message, title, POPUP_INFO);
}

bool Device::popupWarningChoice(std::string_view message, std::string_view title) {
    return Device::popupChoice(message, title, POPUP_WARNING);
}

bool Device::popupErrorChoice(std::string_view message, std::string_view title) {
    return Device::popupChoice(message, title, POPUP_ERROR);
}
