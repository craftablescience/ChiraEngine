#include "Window.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <fmt/core.h>

#include <config/Config.h>
#include <config/ConEntry.h>
#include <core/Engine.h>
#include <event/Events.h>
#include <input/InputManager.h>
#include <render/texture/Image.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <render/material/MaterialFramebuffer.h>
#include <render/material/MaterialTextured.h>
#include <ui/Font.h>
#include <ui/IPanel.h>

using namespace chira;

CHIRA_CREATE_LOG(WINDOW);

[[maybe_unused]]
static ConCommand win_setpos{"win_setpos", "Set the X and Y position of the engine window, (0,0) being at the top left. If no arguments are given, places it in the center of the screen.", [](ConCommand::CallbackArgs args) { // NOLINT(cert-err58-cpp)
    if (args.empty()) {
        Engine::getWindow()->moveToCenter();
    } else if (args.size() >= 2) {
        Engine::getWindow()->moveToPosition({static_cast<int>(std::stoi(args[0])), static_cast<int>(std::stoi(args[1]))});
    }
}};

static ConVar win_width{"win_width", 1280, "The width of the engine window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    Engine::getWindow()->setFrameSize({static_cast<int>(std::stoi(newValue.data())), Engine::getWindow()->getFrameSize().y});
}};

static ConVar win_height{"win_height", 720, "The height of the engine window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    Engine::getWindow()->setFrameSize({Engine::getWindow()->getFrameSize().x, static_cast<int>(std::stoi(newValue.data()))});
}};

static ConVar win_fullscreen{"win_fullscreen", false, "If the window is in fullscreen. Overrides \"win_start_maximized\" if true at startup.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    Engine::getWindow()->setFullscreen(static_cast<bool>(std::stoi(newValue.data())));
}};

static ConVar win_vsync{"win_vsync", true, "Limit the FPS to your monitor's resolution.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    glfwSwapInterval(static_cast<bool>(std::stoi(newValue.data())));
}};

static ConVar input_raw_mouse_motion{"input_raw_mouse_motion", true, "Get more accurate mouse motion.", CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)

bool Window::createGLFWWindow(std::string_view title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Config::GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Config::GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_VISIBLE, this->visible ? GLFW_TRUE : GLFW_FALSE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    this->window = glfwCreateWindow(this->width, this->height, title.data(), nullptr, nullptr);
    if (!this->window) {
        LOG_WINDOW.error("Window creation failed!");
        return false;
    }
    glfwSetWindowUserPointer(this->window, this);
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        LOG_WINDOW.error(fmt::format("OpenGL {} must be available to run this program", Config::GL_VERSION_STRING_PRETTY));
        return false;
    }
    glfwSwapInterval(win_vsync.getValue<int>());

    this->setIcon("file://textures/ui/icon.png");

    if (win_fullscreen.getValue<bool>()) {
        this->setFullscreen(true);
    }

    glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(this->window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    if (glfwRawMouseMotionSupported() && input_raw_mouse_motion.getValue<bool>()) {
        glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* /*w*/, int width_, int height_) {
        Engine::getWindow()->setFrameSize({width_, height_});
    });
    glfwSetKeyCallback(this->window, [](GLFWwindow* /*w*/, int key, int /*scancode*/, int action, int /*mods*/) {
        if (action == GLFW_REPEAT) return;
        for (auto& keybind : InputManager::getKeyButtonCallbacks()) {
            if (keybind.getKey() == static_cast<Key>(key) && keybind.getEventType() == static_cast<InputKeyEventType>(action))
                keybind();
        }
    });
    glfwSetMouseButtonCallback(this->window, [](GLFWwindow* /*w*/, int key, int action, int /*mods*/) {
        if (action == GLFW_REPEAT) return;
        for (auto& keybind : InputManager::getMouseButtonCallbacks()) {
            if (keybind.getKey() == static_cast<Key>(key) && keybind.getEventType() == static_cast<InputKeyEventType>(action))
                keybind();
        }
    });
    glfwSetCursorPosCallback(this->window, [](GLFWwindow* w, double xPos, double yPos) {
        auto* window_ = static_cast<Window*>(glfwGetWindowUserPointer(w));
        auto& lastMX = window_->lastMouseX;
        auto& lastMY = window_->lastMouseY;

        if (lastMX == -1) lastMX = xPos;
        if (lastMY == -1) lastMY = yPos;

        int width, height;
        glfwGetWindowSize(w, &width, &height);
        double xOffset = xPos - lastMX;
        double yOffset = yPos - lastMY;

        for (auto& movebind : InputManager::getMouseMovementCallbacks()) {
            if (movebind.getType() == InputMouseMovementEventType::MOVE)
                movebind(xOffset, yOffset);
        }

        lastMX = xPos;
        lastMY = yPos;
    });
    glfwSetScrollCallback(this->window, [](GLFWwindow* /*w*/, double x, double y) {
        for (auto& movebind : InputManager::getMouseMovementCallbacks()) {
            if (movebind.getType() == InputMouseMovementEventType::SCROLL)
                movebind(x,y);
        }
    });
    glfwSetWindowIconifyCallback(this->window, [](GLFWwindow* w, int isIconified) {
        static_cast<Window*>(glfwGetWindowUserPointer(w))->iconified = (isIconified == GLFW_TRUE);
    });
    glfwSetDropCallback(this->window, [](GLFWwindow* /*w*/, int count, const char** paths) {
        std::vector<std::string> files;
        files.reserve(count);
        for (int i = 0; i < count; i++)
            files.emplace_back(paths[i]);
        Events::createEvent("chira::window::files_dropped", files);
    });

    this->imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(this->imguiContext);
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_DockingEnable;
<<<<<<< HEAD
    // swap ImGui's config dir to prevent bundle crashes
    std::string ConfigPath;
    ConfigPath.append(Engine::getConfigDir());
    ConfigPath.append("imgui.ini");
    io.IniFilename = ConfigPath.c_str();
=======
    this->setImGuiConfigPath();
>>>>>>> f642c7d392fbeaf247f83d1b30a964d55ca86039

    ImGui_ImplGlfw_InitForOpenGL(this->window, true); // register for default input binds
    ImGui_ImplOpenGL3_Init(Config::GL_VERSION_STRING.data());

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
    glfwMakeContextCurrent(this->window);
    ImGui::SetCurrentContext(this->imguiContext);

<<<<<<< HEAD
    // swap ImGui's config dir to prevent bundle crashes
    std::string ConfigPath;
    ConfigPath.append(Engine::getConfigDir());
    ConfigPath.append("imgui.ini");
    ImGui::GetIO().IniFilename = ConfigPath.c_str();
=======
    this->setImGuiConfigPath();
>>>>>>> f642c7d392fbeaf247f83d1b30a964d55ca86039

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
<<<<<<< HEAD
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
=======
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_PassthruCentralNode);
>>>>>>> f642c7d392fbeaf247f83d1b30a964d55ca86039

    Frame::render(this->fboHandle, this->width, this->height);

    for (auto* panel : this->panels) {
        panel->render();
    }

    glDisable(GL_DEPTH_TEST);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    this->surface.render(glm::identity<glm::mat4>());
    glfwSwapBuffers(this->window);
}

Window::~Window() {
    this->removeAllPanels();
    ImGui::SetCurrentContext(this->imguiContext);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(this->imguiContext);
    glfwDestroyWindow(this->window);
}

unsigned int Window::addPanel(IPanel* panel) {
    for (unsigned int i = 0; i < this->panels.size(); i++) {
        if (!this->panels.at(i)) {
            this->panels[i] = panel;
            return i;
        }
    }
    this->panels.push_back(panel);
    return this->panels.size() - 1;
}

IPanel* Window::getPanel(unsigned int panelID) const {
    if (this->panels.size() > panelID)
        return this->panels.at(panelID);
    return nullptr;
}

void Window::removePanel(unsigned int panelID) {
    if (this->panels.size() > panelID) {
        delete this->panels.at(panelID);
        this->panels[panelID] = nullptr;
    }
}

void Window::removeAllPanels() {
    for (const auto* panel : this->panels) {
        delete panel;
    }
    this->panels.clear();
}

void Window::setFrameSize(glm::vec2i newSize) {
    Frame::setFrameSize(newSize);
    glfwSetWindowSize(this->window, this->width, this->height);
    win_width.setValue(this->width, false);
    win_height.setValue(this->height, false);
}

glm::vec2d Window::getMousePosition() const {
    double x = -1.0, y = -1.0;
    glfwGetCursorPos(this->window, &x, &y);
    return {x, y};
}

void Window::captureMouse(bool capture) {
    this->mouseCaptured = capture;
    if (capture) {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    } else {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
        glfwShowWindow(this->window);
    } else {
        glfwHideWindow(this->window);
    }
    Frame::setVisible(visibility);
}

void Window::setFullscreen(bool goFullscreen) const {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowMonitor(this->window, goFullscreen ? glfwGetPrimaryMonitor() : nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
    if (!goFullscreen) {
        this->moveToCenter();
    }
    win_fullscreen.setValue(goFullscreen, false);
}

bool Window::isFullscreen() const {
    return static_cast<bool>(glfwGetWindowMonitor(this->window));
}

void Window::setMaximized(bool maximize) {
    if (!this->isFullscreen()) {
        if (maximize) {
            glfwMaximizeWindow(this->window);
        } else {
            glfwRestoreWindow(this->window);
        }
        int width_, height_;
        glfwGetWindowSize(this->window, &width_, &height_);
        Frame::setFrameSize({width, height});
    }
}

bool Window::isMaximized() const {
    return static_cast<bool>(glfwGetWindowAttrib(this->window, GLFW_MAXIMIZED));
}

void Window::moveToPosition(glm::vec2i pos) const {
    if (this->isFullscreen())
        return;
    glfwSetWindowPos(this->window, pos.x, pos.y);
}

void Window::moveToCenter() const {
    if (this->isFullscreen())
        return;
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(this->window, (mode->width - this->width) / 2, (mode->height - this->height) / 2);
}

void Window::setIcon(const std::string& identifier) const {
    GLFWimage images[1];
    int iconWidth, iconHeight, bitsPerPixel;
    auto icon = Image::getUncompressedImage(FilesystemResourceProvider::getResourceAbsolutePath(identifier), &iconWidth, &iconHeight, &bitsPerPixel, 4, false);
    runtime_assert(icon.data, "Window icon has no data");
    images[0].width = iconWidth;
    images[0].height = iconHeight;
    images[0].pixels = icon.data;
    glfwSetWindowIcon(this->window, 1, images);
}

void Window::shouldStopAfterThisFrame(bool yes) const {
    glfwSetWindowShouldClose(this->window, yes);
}

void Window::displaySplashScreen() {
    glfwMakeContextCurrent(this->window);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fboHandle);
    glClear(GL_COLOR_BUFFER_BIT);
    MeshDataBuilder plane;
    plane.addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    plane.setMaterial(Resource::getResource<MaterialTextured>("file://materials/splashscreen.json").castAssert<IMaterial>());
    plane.render(glm::identity<glm::mat4>());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    this->surface.render(glm::identity<glm::mat4>());
    glfwSwapBuffers(this->window);
}

void Window::setImGuiConfigPath() {
    static std::string configPath = std::string{Config::getConfigDirectory().data()} + "imgui.ini";
    ImGui::GetIO().IniFilename = configPath.c_str();
}
