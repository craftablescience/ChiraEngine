#include "Window.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>
// todo(render): move to render backend
#include <glad/gl.h>
#include <glad/glversion.h>
#include <SDL.h>

#include <config/Config.h>
#include <config/ConEntry.h>
#include <core/Engine.h>
#include <event/Events.h>
#include <input/InputManager.h>
#include <loader/image/Image.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <render/material/MaterialFramebuffer.h>
#include <render/material/MaterialTextured.h>
#include <ui/Font.h>
#include <ui/IPanel.h>

using namespace chira;

CHIRA_CREATE_LOG(WINDOW);

[[maybe_unused]]
ConCommand r_gl_version{"r_gl_version", "Print the current OpenGL version.", [](ConCommand::CallbackArgs /*args*/) { // NOLINT(cert-err58-cpp)
    LOG_WINDOW.infoImportant(GL_VERSION_STRING_PRETTY);
}};

[[maybe_unused]]
ConCommand win_setpos{"win_setpos", "Set the X and Y position of the engine window, (0,0) being at the top left. If no arguments are given, places it in the center of the screen.", [](ConCommand::CallbackArgs args) { // NOLINT(cert-err58-cpp)
    if (args.empty()) {
        Engine::getWindow()->moveToCenter();
    } else if (args.size() >= 2) {
        Engine::getWindow()->moveToPosition({static_cast<int>(std::stoi(args[0])), static_cast<int>(std::stoi(args[1]))});
    }
}};

ConVar win_width{"win_width", 1280, "The width of the engine window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    Engine::getWindow()->setFrameSize({static_cast<int>(std::stoi(newValue.data())), Engine::getWindow()->getFrameSize().y});
}};

ConVar win_height{"win_height", 720, "The height of the engine window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    Engine::getWindow()->setFrameSize({Engine::getWindow()->getFrameSize().x, static_cast<int>(std::stoi(newValue.data()))});
}};

ConVar win_fullscreen{"win_fullscreen", false, "If the window is in fullscreen. Overrides \"win_start_maximized\" if true at startup.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    Engine::getWindow()->setFullscreen(static_cast<bool>(std::stoi(newValue.data())));
}};

ConVar win_vsync{"win_vsync", true, "Limit the FPS to your monitor's resolution.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    if (win_vsync.getValue<bool>()) {
        // Fall back to regular vsync if we don't have adaptive vsync
        if (SDL_GL_SetSwapInterval(-1) == -1) {
            SDL_GL_SetSwapInterval(1);
        }
    } else {
        SDL_GL_SetSwapInterval(0);
    }
}};

ConVar input_raw_mouse_motion{"input_raw_mouse_motion", true, "Get more accurate mouse motion.", CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)

bool Window::createGLFWWindow(std::string_view title) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_VERSION_MINOR);
#ifdef DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG | SDL_GL_CONTEXT_DEBUG_FLAG);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif
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
            (this->visible ? SDL_WINDOW_SHOWN : SDL_WINDOW_HIDDEN);
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

    if (win_fullscreen.getValue<bool>()) {
        this->setFullscreen(true);
    }

    // todo: initialize convars here
    if (win_vsync.getValue<bool>()) {
        // Try enabling adaptive vsync
        if (SDL_GL_SetSwapInterval(-1) == -1) {
            SDL_GL_SetSwapInterval(1);
        }
    } else {
        SDL_GL_SetSwapInterval(0);
    }

//    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* /*w*/, int width_, int height_) {
//        Engine::getWindow()->setFrameSize({width_, height_});
//    });
//    glfwSetKeyCallback(this->window, [](GLFWwindow* /*w*/, int key, int /*scancode*/, int action, int /*mods*/) {
//        if (action == GLFW_REPEAT) return;
//        for (auto& keybind : InputManager::getKeyButtonCallbacks()) {
//            if (keybind.getKey() == static_cast<Key>(key) && keybind.getEventType() == static_cast<InputKeyEventType>(action))
//                keybind();
//        }
//    });
//    glfwSetMouseButtonCallback(this->window, [](GLFWwindow* /*w*/, int key, int action, int /*mods*/) {
//        if (action == GLFW_REPEAT) return;
//        for (auto& keybind : InputManager::getMouseButtonCallbacks()) {
//            if (keybind.getKey() == static_cast<Key>(key) && keybind.getEventType() == static_cast<InputKeyEventType>(action))
//                keybind();
//        }
//    });
//    glfwSetCursorPosCallback(this->window, [](GLFWwindow* w, double xPos, double yPos) {
//        auto* window_ = static_cast<Window*>(glfwGetWindowUserPointer(w));
//        auto& lastMX = window_->lastMouseX;
//        auto& lastMY = window_->lastMouseY;
//
//        if (lastMX == -1) lastMX = xPos;
//        if (lastMY == -1) lastMY = yPos;
//
//        int width, height;
//        glfwGetWindowSize(w, &width, &height);
//        double xOffset = xPos - lastMX;
//        double yOffset = yPos - lastMY;
//
//        for (auto& movebind : InputManager::getMouseMovementCallbacks()) {
//            if (movebind.getType() == InputMouseMovementEventType::MOVE)
//                movebind(xOffset, yOffset);
//        }
//
//        lastMX = xPos;
//        lastMY = yPos;
//    });
//    glfwSetScrollCallback(this->window, [](GLFWwindow* /*w*/, double x, double y) {
//        for (auto& movebind : InputManager::getMouseMovementCallbacks()) {
//            if (movebind.getType() == InputMouseMovementEventType::SCROLL)
//                movebind(x,y);
//        }
//    });
//    glfwSetWindowIconifyCallback(this->window, [](GLFWwindow* w, int isIconified) {
//        static_cast<Window*>(glfwGetWindowUserPointer(w))->iconified = (isIconified == GLFW_TRUE);
//    });
//    glfwSetDropCallback(this->window, [](GLFWwindow* /*w*/, int count, const char** paths) {
//        std::vector<std::string> files;
//        files.reserve(count);
//        for (int i = 0; i < count; i++)
//            files.emplace_back(paths[i]);
//        Events::createEvent("chira::window::files_dropped", files);
//    });

    this->imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(this->imguiContext);
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_DockingEnable;
    Window::setImGuiConfigPath();

    ImGui_ImplSDL2_InitForOpenGL(this->window, this->glContext);
    ImGui_ImplOpenGL3_Init(GL_VERSION_STRING.data());

    auto defaultFont = Resource::getUniqueResource<Font>("file://fonts/default.json");
    ImGui::GetIO().FontDefault = defaultFont->getFont();

    return true;
}

static void makeSurface(Window* window, MeshDataBuilder* surface) {
    surface->addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    surface->setMaterial(Resource::getResource<MaterialFramebuffer>("file://materials/window.json", window).castAssert<IMaterial>());
}

Window::Window(std::string name_, std::string_view title)
    : Frame(std::move(name_), win_width.getValue<int>(), win_height.getValue<int>(), {}, true, false) {
    this->visible = false;
    if (this->createGLFWWindow(title)) {
        this->createFramebuffer();
        makeSurface(this, &this->surface);
    }
}

Window::Window(std::string_view title)
    : Frame(win_width.getValue<int>(), win_height.getValue<int>(), {}, true, false) {
    this->visible = false;
    if (this->createGLFWWindow(title)) {
        this->createFramebuffer();
        makeSurface(this, &this->surface);
    }
}

void Window::render(glm::mat4 /*parentTransform*/) {
    SDL_GL_MakeCurrent(this->window, this->glContext);
    ImGui::SetCurrentContext(this->imguiContext);

    Window::setImGuiConfigPath();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(this->window);
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_PassthruCentralNode);

    Frame::render(glm::identity<glm::mat4>());
    glViewport(0, 0, this->width, this->height);

    for (auto& [uuid, panel] : this->panels) {
        panel->render();
    }

    glDisable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, this->handle.fboHandle);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    this->surface.render(glm::identity<glm::mat4>());

    glEnable(GL_DEPTH_TEST);

    SDL_GL_SwapWindow(this->window);
}

Window::~Window() {
    this->removeAllPanels();
    ImGui::SetCurrentContext(this->imguiContext);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(this->imguiContext);
    SDL_GL_DeleteContext(this->glContext);
    SDL_DestroyWindow(this->window);
}

uuids::uuid Window::addPanel(IPanel* panel) {
    const auto uuid = UUIDGenerator::getNewUUID();
    this->panels[uuid] = panel;
    return uuid;
}

IPanel* Window::getPanel(const uuids::uuid& panelID) {
    if (this->panels.count(panelID) > 0)
        return this->panels[panelID];
    return nullptr;
}

void Window::removePanel(const uuids::uuid& panelID) {
    if (this->panels.count(panelID) > 0) {
        delete this->panels[panelID];
        this->panels.erase(panelID);
    }
}

void Window::removeAllPanels() {
    for (const auto& [panelID, panel] : this->panels) {
        delete panel;
    }
    this->panels.clear();
}

void Window::setFrameSize(glm::vec2i newSize) {
    Frame::setFrameSize(newSize);
    SDL_SetWindowSize(this->window, this->width, this->height);
    win_width.setValue(this->width, false);
    win_height.setValue(this->height, false);
}

glm::vec2i Window::getMousePosition() {
    int x = -1, y = -1;
    SDL_GetMouseState(&x, &y);
    return {x, y};
}

void Window::captureMouse(bool capture) {
    this->mouseCaptured = capture;
    if (capture) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    } else {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
}

bool Window::isMouseCaptured() const {
    return this->mouseCaptured;
}

bool Window::isIconified() const {
    return this->iconified;
}

void Window::setVisible(bool visibility) {
    if (visibility) {
        SDL_ShowWindow(this->window);
    } else {
        SDL_HideWindow(this->window);
    }
    Frame::setVisible(visibility);
}

void Window::setFullscreen(bool goFullscreen) const {
    if (!goFullscreen) {
        this->moveToCenter();
    }
    win_fullscreen.setValue(goFullscreen, false);
}

bool Window::isFullscreen() const {
    return SDL_GetWindowFlags(this->window) & SDL_WINDOW_FULLSCREEN;
}

void Window::setMaximized(bool maximize) {
    if (!this->isFullscreen()) {
        if (maximize) {
            SDL_MaximizeWindow(this->window);
        } else {
            SDL_RestoreWindow(this->window);
        }
        SDL_GetWindowSize(this->window, &this->width, &this->height);
        Frame::setFrameSize({this->width, this->height});
    }
}

bool Window::isMaximized() const {
    return SDL_GetWindowFlags(this->window) & SDL_WINDOW_MAXIMIZED;
}

void Window::moveToPosition(glm::vec2i pos) const {
    if (this->isFullscreen())
        return;
    SDL_SetWindowPosition(this->window, pos.x, pos.y);
}

void Window::moveToCenter() const {
    if (this->isFullscreen())
        return;
    SDL_Rect rect;
    SDL_GetDisplayBounds(0, &rect);
    SDL_SetWindowPosition(this->window, rect.w / 2, rect.h / 2);
}

void Window::setIcon(const std::string& identifier) const {
    int iconWidth, iconHeight, bitsPerPixel;
    auto* icon = Image::getUncompressedImage(FilesystemResourceProvider::getResourceAbsolutePath(identifier), &iconWidth, &iconHeight, &bitsPerPixel, 4, false);
    runtime_assert(icon, "Window icon has no data");
    auto* sdlIcon = SDL_CreateRGBSurfaceWithFormatFrom(icon, iconWidth, iconHeight, bitsPerPixel, 8, SDL_PIXELFORMAT_RGBA8888);
    SDL_SetWindowIcon(this->window, sdlIcon);
    SDL_FreeSurface(sdlIcon);
    Image::deleteUncompressedImage(icon);
}

void Window::shouldCloseAfterThisFrame(bool yes) {
    this->shouldClose = yes;
}

void Window::displaySplashScreen() {
    SDL_GL_MakeCurrent(this->window, this->glContext);
    Renderer::pushFrameBuffer(this->handle);
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

void Window::setImGuiConfigPath() {
    static std::string configPath = Config::getConfigFile("imgui.ini");
    ImGui::GetIO().IniFilename = configPath.c_str();
}
