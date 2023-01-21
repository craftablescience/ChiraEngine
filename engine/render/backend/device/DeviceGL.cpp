#include "DeviceGL.h"

// todo(render): move to render backend
#include <glad/gl.h>
#include <glad/glversion.h>
#include <imgui.h>
#include <SDL.h>

#include <config/Config.h>
#include <config/ConEntry.h>
#include <core/Engine.h>
#include <event/Events.h>
#include <loader/image/Image.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <render/material/MaterialFramebuffer.h>
#include <render/material/MaterialTextured.h>
#include <ui/Font.h>
#include <ui/IPanel.h>

using namespace chira;

CHIRA_CREATE_LOG(WINDOW);

[[maybe_unused]]
ConCommand win_setpos{"win_setpos", "Set the X and Y position of the main window, (0,0) being at the top left. If no arguments are given, places it in the center of the screen.", [](ConCommand::CallbackArgs args) {
    if (args.empty()) {
        Engine::getDevice()->moveToCenter();
    } else if (args.size() >= 2) {
        Engine::getDevice()->moveToPosition({static_cast<int>(std::stoi(args[0])), static_cast<int>(std::stoi(args[1]))});
    }
}};

ConVar win_width{"win_width", 1280, "The width of the main window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    Engine::getDevice()->setSize({static_cast<int>(std::stoi(newValue.data())), Engine::getDevice()->getFrame()->getFrameSize().y});
}};

ConVar win_height{"win_height", 720, "The height of the main window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    Engine::getDevice()->setSize({Engine::getDevice()->getFrame()->getFrameSize().x, static_cast<int>(std::stoi(newValue.data()))});
}};

ConVar win_maximized{"win_maximized", true, "If the main window is maximized.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    Engine::getDevice()->setMaximized(static_cast<bool>(std::stoi(newValue.data())));
}};

ConVar win_fullscreen{"win_fullscreen", false, "If the main window is fullscreen. Overrides \"win_maximized\" if true.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    Engine::getDevice()->setFullscreen(static_cast<bool>(std::stoi(newValue.data())));
}};

ConVar win_vsync{"win_vsync", true, "Limit the FPS to your monitor's resolution.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    if (!static_cast<bool>(std::stoi(newValue.data()))) {
        SDL_GL_SetSwapInterval(0);
        return;
    } else if (SDL_GL_SetSwapInterval(-1) == -1) {
        // Fall back to regular vsync if we don't have adaptive vsync
        SDL_GL_SetSwapInterval(1);
    }
}};

[[maybe_unused]]
ConVar input_raw_mouse_motion{"input_raw_mouse_motion", true, "Get more accurate mouse motion.", CON_FLAG_CACHE};

static void setImGuiConfigPath() {
    static std::string configPath = Config::getConfigFile("imgui.ini");
    ImGui::GetIO().IniFilename = configPath.c_str();
}

bool Device::createGLFWWindow(std::string_view title) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    int glContextFlags;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &glContextFlags);
    glContextFlags |= SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#ifdef DEBUG
    glContextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glContextFlags);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    int windowFlags =
            SDL_WINDOW_OPENGL |
            SDL_WINDOW_ALLOW_HIGHDPI |
            SDL_WINDOW_RESIZABLE |
            (this->visible ? SDL_WINDOW_SHOWN : SDL_WINDOW_HIDDEN);
    if (win_maximized.getValue<bool>()) {
        windowFlags |= SDL_WINDOW_MAXIMIZED;
    }
    if (win_fullscreen.getValue<bool>()) {
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    this->window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, windowFlags);
    if (!this->window) {
        LOG_WINDOW.error("Window creation failed!");
        return false;
    }
    this->glContext = SDL_GL_CreateContext(this->window);
    SDL_GL_MakeCurrent(this->window, this->glContext);

    if (!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {
        LOG_WINDOW.error("OpenGL {} must be available to run this program", GL_VERSION_STRING_PRETTY);
        return false;
    }

    this->setIcon("file://textures/ui/icon.png");

    // looks ugly, but we can't call this stuff because the engine is still creating us
    // fix this in the rewrite
    if (win_vsync.getValue<bool>()) {
        // Try enabling adaptive vsync
        if (SDL_GL_SetSwapInterval(-1) == -1) {
            SDL_GL_SetSwapInterval(1);
        }
    } else {
        SDL_GL_SetSwapInterval(0);
    }

    this->imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(this->imguiContext);
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_DockingEnable;
    setImGuiConfigPath();

    Renderer::initImGui(this->window, this->glContext);

    auto defaultFont = Resource::getUniqueResource<Font>("file://fonts/default.json");
    ImGui::GetIO().FontDefault = defaultFont->getFont();

    return true;
}

Device::Device(std::string_view title) : frame(win_width.getValue<int>(), win_height.getValue<int>(), {}, true, false) {
    this->width = win_width.getValue<int>();
    this->height = win_height.getValue<int>();
    this->frame.setVisible(false);
    bool success = this->createGLFWWindow(title);
    runtime_assert(success, "Could not create SDL window!");
    if (success) {
        this->frame.recreateFramebuffer();
        this->surface.addSquare({}, {2, -2}, SignedAxis::ZN, 0);
        this->surface.setMaterial(Resource::getResource<MaterialFramebuffer>("file://materials/window.json", &this->frame).castAssert<IMaterial>());
    }
}

void Device::refresh() {
    SDL_GL_MakeCurrent(this->window, this->glContext);
    ImGui::SetCurrentContext(this->imguiContext);

    setImGuiConfigPath();

    Renderer::startImGuiFrame(this->window);

    this->frame.update();
    this->frame.render(glm::identity<glm::mat4>());
    glViewport(0, 0, this->width, this->height);

    for (auto& [uuid, panel] : this->panels) {
        panel->render();
    }

    glDisable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, this->frame.getRawHandle().fboHandle);
    Renderer::endImGuiFrame();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    this->surface.render(glm::identity<glm::mat4>());

    glEnable(GL_DEPTH_TEST);

    SDL_GL_SwapWindow(this->window);
}

Device::~Device() {
    this->removeAllPanels();
    ImGui::SetCurrentContext(this->imguiContext);
    Renderer::destroyImGui();
    ImGui::DestroyContext(this->imguiContext);
    SDL_GL_DeleteContext(this->glContext);
    SDL_DestroyWindow(this->window);
}

Frame* Device::getFrame() {
    return &this->frame;
}

uuids::uuid Device::addPanel(IPanel* panel) {
    const auto uuid = UUIDGenerator::getNewUUID();
    this->panels[uuid] = panel;
    return uuid;
}

IPanel* Device::getPanel(const uuids::uuid& panelID) {
    if (this->panels.count(panelID) > 0)
        return this->panels[panelID];
    return nullptr;
}

void Device::removePanel(const uuids::uuid& panelID) {
    if (this->panels.count(panelID) > 0) {
        delete this->panels[panelID];
        this->panels.erase(panelID);
    }
}

void Device::removeAllPanels() {
    for (const auto& [panelID, panel] : this->panels) {
        delete panel;
    }
    this->panels.clear();
}

void Device::setSize(glm::vec2i newSize, bool setWindowSize /*= true*/) {
    this->width = newSize.x;
    this->height = newSize.y;
    this->frame.setFrameSize(newSize);
    if (setWindowSize)
        SDL_SetWindowSize(this->window, newSize.x, newSize.y);
    win_width.setValue(this->width, false);
    win_height.setValue(this->height, false);
}

glm::vec2i Device::getMousePosition() {
    int x = -1, y = -1;
    SDL_GetMouseState(&x, &y);
    return {x, y};
}

void Device::captureMouse(bool capture) {
    this->mouseCaptured = capture;
    if (capture) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    } else {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
}

bool Device::isMouseCaptured() const {
    return this->mouseCaptured;
}

bool Device::isIconified() const {
    return this->iconified;
}

void Device::setVisible(bool visibility) {
    if (visibility) {
        SDL_ShowWindow(this->window);
    } else {
        SDL_HideWindow(this->window);
    }
    this->frame.setVisible(visibility);
}

void Device::setFullscreen(bool goFullscreen) const {
    if (!goFullscreen) {
        this->moveToCenter();
    }
    SDL_SetWindowFullscreen(this->window, goFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    win_fullscreen.setValue(goFullscreen, false);
}

bool Device::isFullscreen() const {
    return SDL_GetWindowFlags(this->window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
}

void Device::setMaximized(bool maximize) {
    if (!this->isFullscreen()) {
        if (maximize) {
            SDL_MaximizeWindow(this->window);
        } else {
            SDL_RestoreWindow(this->window);
        }
        SDL_GetWindowSize(this->window, &this->width, &this->height);
        this->frame.setFrameSize({this->width, this->height});
    }
    win_maximized.setValue(maximize, false);
}

bool Device::isMaximized() const {
    return SDL_GetWindowFlags(this->window) & SDL_WINDOW_MAXIMIZED;
}

void Device::moveToPosition(glm::vec2i pos) const {
    if (this->isFullscreen())
        return;
    SDL_SetWindowPosition(this->window, pos.x, pos.y);
}

void Device::moveToCenter() const {
    if (this->isFullscreen())
        return;
    SDL_Rect rect;
    SDL_GetDisplayBounds(0, &rect);
    SDL_SetWindowPosition(this->window, rect.w / 2, rect.h / 2);
}

void Device::setIcon(const std::string& identifier) const {
    int iconWidth, iconHeight, bitsPerPixel;
    auto* icon = Image::getUncompressedImage(FilesystemResourceProvider::getResourceAbsolutePath(identifier), &iconWidth, &iconHeight, &bitsPerPixel, 4, false);
    runtime_assert(icon, "Window icon has no data");
    auto* sdlIcon = SDL_CreateRGBSurfaceWithFormatFrom(icon, iconWidth, iconHeight, bitsPerPixel, 8, SDL_PIXELFORMAT_RGBA8888);
    SDL_SetWindowIcon(this->window, sdlIcon);
    SDL_FreeSurface(sdlIcon);
    Image::deleteUncompressedImage(icon);
}

bool Device::shouldCloseAfterThisFrame() const {
    return this->shouldClose;
}

void Device::closeAfterThisFrame(bool yes /*= true*/) {
    this->shouldClose = yes;
}

void Device::displaySplashScreen() {
    SDL_GL_MakeCurrent(this->window, this->glContext);
    Renderer::pushFrameBuffer(this->frame.getRawHandle());
    MeshDataBuilder plane;
    plane.addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    plane.setMaterial(Resource::getResource<MaterialTextured>("file://materials/splashscreen.json").castAssert<IMaterial>());
    plane.render(glm::identity<glm::mat4>());
    glDisable(GL_DEPTH_TEST);
    Renderer::popFrameBuffer();
    glViewport(0, 0, this->width, this->height);
    this->surface.render(glm::identity<glm::mat4>());
    glEnable(GL_DEPTH_TEST);
    SDL_GL_SwapWindow(this->window);
}
