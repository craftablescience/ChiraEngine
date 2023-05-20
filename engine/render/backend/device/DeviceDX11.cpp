// TODO: Create a pure DX11/D3D11 Device at some point!

// Resources used:
// https://stackoverflow.com/questions/53269015/how-can-i-create-direct3d-11-renderer-using-sdl-2-0-9
// https://learn.microsoft.com/en-us/windows/win32/direct3d11/dx-graphics-overviews
// https://www.gamedev.net/forums/topic/699741-how-can-i-create-direct3d-11-renderer-using-sdl-209/
// https://gist.github.com/YukiSnowy/06ad3adadd38484cb1377ea45152c1a6

#include "DeviceDX11.h"

#include <array>

#include <backends/imgui_impl_sdl2.h>
#include <d3d11.h>       // D3D interface
#include <dxgi.h>        // DirectX driver interface

#include <imgui.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <config/Config.h>
#include <config/ConEntry.h>
#include <event/Events.h>
#include <i18n/TranslationManager.h>
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

// TODO: This sets the **GL** swap interval! Not DX! Figure out how to do it for DX!
static void setVSync(bool enable) {
    if (!enable) {
        SDL_GL_SetSwapInterval(0);
    } else if (SDL_GL_SetSwapInterval(-1) == -1) {
        // Fall back to regular vsync if we don't have adaptive vsync
        SDL_GL_SetSwapInterval(1);
    }
}

ConVar win_vsync{ "win_vsync", true, "Limit the FPS to your monitor's resolution.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    setVSync(static_cast<bool>(std::stoi(newValue.data())));
} };

[[maybe_unused]]
ConVar input_raw_mouse_motion{ "input_raw_mouse_motion", true, "Get more accurate mouse motion.", CON_FLAG_CACHE };

static void setImGuiConfigPath() {
    static std::string configPath = Config::getConfigFile("imgui.ini");
    ImGui::GetIO().IniFilename = configPath.c_str();
}

Renderer::FrameBufferHandle g_WindowFramebufferHandle{};
SDL_Renderer* g_Renderer;
/* TODO: See the other reference to these
ID3D11Device* g_D3D11Device;
ID3D11DeviceContext* g_D3D11Context;
*/

bool Device::initBackendAndCreateSplashscreen(bool splashScreenVisible) {
    static bool alreadyRan = false;
    if (alreadyRan)
        return false;
    alreadyRan = true;

    SDL_SetMainReady();

#ifdef CHIRA_PLATFORM_WINDOWS
    // Force enable DPI awareness because the manifest method didn't work
    SDL_SetHintWithPriority(SDL_HINT_WINDOWS_DPI_SCALING, "0", SDL_HINT_OVERRIDE);
    SDL_SetHintWithPriority(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2", SDL_HINT_OVERRIDE);
#endif

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER)) {
        LOG_WINDOW.error("SDL2 failed to initialize: {}", SDL_GetError());
        return false;
    }

    int windowFlags =
        SDL_WINDOW_ALLOW_HIGHDPI |
        SDL_WINDOW_ALWAYS_ON_TOP |
        SDL_WINDOW_BORDERLESS |
        (splashScreenVisible ? SDL_WINDOW_SHOWN : SDL_WINDOW_HIDDEN);
    int width = 640, height = 480;

    // make sure we set the render driver to d3d11
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11");

    g_Splashscreen = SDL_CreateWindow("Loading...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
    if (!g_Splashscreen) {
        LOG_WINDOW.error("Splashscreen window creation failed! Error: {}", SDL_GetError());
        return false;
    }

    // Technically we don't actually need to use SDL_Renderer but it's an easy solution
    // for creating a D3D11 renderer. Which is super helpful.
    g_Renderer = SDL_CreateRenderer(g_Splashscreen, -1, SDL_RENDERER_ACCELERATED);

    // Make sure the renderer didn't fail to initialize
    if (!renderer) {
        LOG_WINDOW.error("Failed to initialize SDL Renderer! Error: {}", SDL_GetError());
        return false;
    }

    // Check if we even have a D3D11 renderer before anything else
    SDL_RendererInfo info;
    SDL_GetRendererInfo(g_Renderer, &info);
    if (info.name != std::string("direct3d11")) {
        LOG_WINDOW.error("Direct3D 11 must be available to run this program!");
        return false;
    }

    // TODO: Re-enable these checks when I actually know how to clear these variables out.
    /*// attempt to obtain device and context
    g_D3D11Device = SDL_RenderGetD3D11Device();
    g_D3D11Context = g_D3D11Device->GetImmediateContext();

    if (!g_D3D11Device) {
        LOG_WINDOW.error("Failed to obtain D3D11 Device! Error: {}", SDL_GetError());
        return false;
    }
    if (!g_D3D11Context) {
        LOG_WINDOW.error("Failed to obtain D3D11 Context!");
        return false;
    }*/
    setVSync(win_vsync.getValue<bool>());

    Renderer::pushFrameBuffer(g_WindowFramebufferHandle);
    Renderer::recreateFrameBuffer(&g_WindowFramebufferHandle, width, height, WrapMode::REPEAT, WrapMode::REPEAT, FilterMode::LINEAR, true);
    glViewport(0, 0, width, height);

    MeshDataBuilder plane;
    plane.addSquare({}, { 2, -2 }, SignedAxis::ZN, 0);
    plane.setMaterial(Resource::getResource<MaterialTextured>("file://materials/splashscreen.json").castAssert<IMaterial>());
    plane.render(glm::identity<glm::mat4>());

    // TODO: Figure out how to disable depth test in D3D11
    //glDisable(GL_DEPTH_TEST);
    Renderer::popFrameBuffer();

    MeshDataBuilder windowSurface;
    windowSurface.addSquare({}, { 2, -2 }, SignedAxis::ZN, 0);
    windowSurface.setMaterial(Resource::getResource<MaterialFrameBuffer>("file://materials/window.json", &g_WindowFramebufferHandle).castAssert<IMaterial>());
    windowSurface.render(glm::identity<glm::mat4>());

    return true;
}

void Device::destroySplashscreen() {
    if (g_Splashscreen) {
        SDL_DestroyRenderer(g_Renderer);
        SDL_DestroyWindow(g_Splashscreen);
        g_Splashscreen = nullptr;
    }
}

void Device::destroyBackend() {
    Renderer::destroyImGui();
    Device::destroyAllWindows();
    SDL_Quit();
}

std::uint64_t Device::getTicks() {
    return SDL_GetTicks64();
}

// TODO: This function is probably complete garbage. Unlike the OGL renderer, this renderer
// currently doesn't share contexts and instead creates a new renderer for every window.
// which means the window handle is not the same between renderes to compensate for the need
// to store a renderer in the handle.
[[nodiscard]] Device::WindowHandle* Device::createWindow(int width, int height, std::string_view title, Layer* layer) {
    int freeWindow = findFreeWindow();
    if (freeWindow == -1)
        return nullptr;

    g_Windows[freeWindow] = WindowHandle{};
    WindowHandle& handle = g_Windows.at(freeWindow);

    handle.width = width;
    handle.height = height;

    int windowFlags =
        SDL_WINDOW_ALLOW_HIGHDPI |
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_SHOWN;
    handle.window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
    if (!handle.window) {
        LOG_WINDOW.error("Window creation failed! Error: {}", SDL_GetError());
        return nullptr;
    }
    SDL_SetWindowData(handle.window, "handle", &handle);

    handle.renderer = SDL_CreateRenderer(handle.window, -1, SDL_RENDERER_ACCELERATED);
    // Make sure the renderer didn't fail to initialize
    if (!renderer) {
        LOG_WINDOW.error("Failed to initialize SDL Renderer! Error: {}", SDL_GetError());
        return false;
    }

    if (layer) {
        handle.layer = layer;
        handle.layerIsSelfOwned = false;
    }
    else {
        handle.layer = new Layer{ {width, height} };
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

    Renderer::initImGui(handle.window, g_D3D11Context, g_D3D11Device);

    static bool bakedFonts = false;
    if (!bakedFonts) {
        bakedFonts = true;

        auto defaultFont = Resource::getUniqueUncachedResource<Font>(TR("resource.font.default"));
        io.FontDefault = defaultFont->getFont();
        io.Fonts->Build();
    }

    return &handle;
}

void Device::refreshWindows() {
    // Render each window
    for (auto& handle : g_Windows) {
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

        // TODO: What does this even do?
        //SDL_GL_MakeCurrent(handle.window, g_GLContext);
        //glViewport(0, 0, handle.width, handle.height);

        ImGui::SetCurrentContext(handle.imguiContext);
        setImGuiConfigPath();

        Renderer::pushFrameBuffer(*handle.layer->getRawHandle());
        Renderer::startImGuiFrame();

        handle.layer->update();
        handle.layer->render();

        for (auto& [uuid, panel] : handle.panels) {
            panel->render();
        }

        // TODO: Figure out how to do this for D3D11
        //glDisable(GL_DEPTH_TEST);

        Renderer::endImGuiFrame();
        Renderer::popFrameBuffer();

        MeshDataBuilder surface;
        surface.addSquare({}, { 2, -2 }, SignedAxis::ZN, 0);
        surface.setMaterial(Resource::getUniqueUncachedResource<MaterialFrameBuffer>("file://materials/window.json", handle.layer->getRawHandle()).castAssert<IMaterial>());
        surface.render(glm::identity<glm::mat4>());

        // TODO: Figure out how to do this for D3D11
        //glEnable(GL_DEPTH_TEST);

        // TODO: What does this even do??
        //SDL_GL_SwapWindow(handle.window);
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
            for (auto& windowHandle : g_Windows) {
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
                handle->layer->setSize({ handle->width, handle->height });
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
    for (const auto& handle : g_Windows) {
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
    }
    else {
        SDL_RestoreWindow(handle->window);
    }
    SDL_GetWindowSize(handle->window, &handle->width, &handle->height);
    handle->layer->setSize({ handle->width, handle->height });
}

[[nodiscard]] bool Device::isWindowMaximized(WindowHandle* handle) {
    return SDL_GetWindowFlags(handle->window) & SDL_WINDOW_MAXIMIZED;
}

void Device::minimizeWindow(WindowHandle* handle, bool minimize) {
    if (minimize) {
        SDL_MinimizeWindow(handle->window);
    }
    else {
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
    }
    else {
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
    handle->layer->setSize({ width, height });
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
    glm::vec2i pos{ -1, -1 };
    SDL_GetGlobalMouseState(&pos.x, &pos.y);
    return pos;
}

[[nodiscard]] glm::vec2i Device::getMousePositionInFocusedWindow() {
    glm::vec2i pos{ -1, -1 };
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
    }
    else {
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
    SDL_DestroyRenderer(handle->renderer);
    SDL_DestroyWindow(handle->window);
    handle->window = nullptr;
}

void Device::destroyAllWindows() {
    for (auto& handle : g_Windows) {
        if (handle) {
            Device::destroyWindow(&handle);
        }
    }
}