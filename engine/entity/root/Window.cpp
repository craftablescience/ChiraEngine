#include "Window.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <config/ConEntry.h>
#include <core/Engine.h>
#include <config/Config.h>
#include <event/Events.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include <loader/image/Image.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <resource/FontResource.h>
#include <render/material/MaterialFramebuffer.h>
#include <ui/IPanel.h>

using namespace chira;

[[maybe_unused]]
static ConVar win_width{"win_width", 1280, "The width of the engine window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    // ConVar is set in registry since it's cached, but that's before the window is created!
    static bool isSetAtAPlaceWhereThisWillErrorOut = true;
    if (!isSetAtAPlaceWhereThisWillErrorOut) {
        try {
            Engine::getWindow()->setFrameSize({static_cast<int>(std::stoi(newValue.data())), Engine::getWindow()->getFrameSize().y});
        } catch (const std::invalid_argument &) {}
        isSetAtAPlaceWhereThisWillErrorOut = false;
    }
}};

[[maybe_unused]]
static ConVar win_height{"win_height", 720, "The height of the engine window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    // ConVar is set in registry since it's cached, but that's before the window is created!
    static bool isSetAtAPlaceWhereThisWillErrorOut = true;
    if (!isSetAtAPlaceWhereThisWillErrorOut) {
        try {
            Engine::getWindow()->setFrameSize({Engine::getWindow()->getFrameSize().x, static_cast<int>(std::stoi(newValue.data()))});
        } catch (const std::invalid_argument &) {}
        isSetAtAPlaceWhereThisWillErrorOut = false;
    }
}};

[[maybe_unused]]
static ConVar win_start_maximized{"win_start_maximized", true, "Start the window maximized. Ignored if \"win_start_fullscreen\" is set to true.", CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)

//todo: doesn't let you switch fullscreen and back at runtime because of some weird bugs
[[maybe_unused]]
static ConVar win_start_fullscreen{"win_start_fullscreen", false, "Start the window in fullscreen. Overrides \"win_start_maximized\".", CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)

[[maybe_unused]]
static ConVar win_vsync{"win_vsync", true, "Limit the FPS to your monitor's resolution.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) { // NOLINT(cert-err58-cpp)
    // ConVar is set in registry since it's cached, but that's before the window is created!
    static bool isSetAtAPlaceWhereThisWillErrorOut = true;
    if (!isSetAtAPlaceWhereThisWillErrorOut) {
        try {
            glfwSwapInterval(static_cast<bool>(std::stoi(newValue.data())));
        } catch (const std::invalid_argument &) {}
        isSetAtAPlaceWhereThisWillErrorOut = false;
    }
}};

[[maybe_unused]]
static ConVar input_raw_mouse_motion{"input_raw_mouse_motion", true, "Get more accurate mouse motion.", CON_FLAG_CACHE}; // NOLINT(cert-err58-cpp)

bool Window::createGLFWWindow(std::string_view title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
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
        Logger::log(LOG_ERROR, "GLFW", TR("error.glfw.window"));
        return false;
    }
    if (win_start_fullscreen.getValue<bool>())
        this->setFullscreen(true);
    else if (win_start_maximized.getValue<bool>()) {
        glfwMaximizeWindow(this->window);
    }

    glfwSetWindowUserPointer(this->window, this);
    glfwMakeContextCurrent(this->window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        Logger::log(LOG_ERROR, "OpenGL", TRF("error.opengl.version", GL_VERSION_STRING_PRETTY));
        return false;
    }
    glfwSwapInterval(win_vsync.getValue<int>());

    this->setIcon("file://textures/ui/icon.png");

    glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(this->window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    if (glfwRawMouseMotionSupported() && input_raw_mouse_motion.getValue<bool>())
        glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* /*w*/, int width_, int height_) {
        win_width.setValue(width_);
        win_height.setValue(height_);
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
    glfwSetWindowMaximizeCallback(this->window, [](GLFWwindow* /*w*/, int isMaximized) {
        win_start_maximized.setValue(static_cast<bool>(isMaximized));
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(this->window, true); // register for default input binds
    ImGui_ImplOpenGL3_Init(GL_VERSION_STRING.data());

    auto defaultFont = Resource::getUniqueResource<FontResource>("file://fonts/default.json");
    ImGui::GetIO().FontDefault = defaultFont->getFont();

    return true;
}

static void makeSurface(Window* window, MeshDataBuilder* surface) {
    surface->addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    surface->setMaterial(Resource::getResource<MaterialFramebuffer>("file://materials/window.json", window).castAssert<MaterialBase>());
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

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    Frame::render(this->fboHandle, this->width, this->height);

    for (auto& [uuid, panel] : this->panels) {
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
    glfwSetWindowSize(this->window, this->width, this->height);
    if (win_width.getValue<int>() != this->width) {
        win_width.setValue(this->width);
    }
    if (win_height.getValue<int>() != this->height) {
        win_height.setValue(this->height);
    }
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
    if (visibility)
        glfwShowWindow(this->window);
    else
        glfwHideWindow(this->window);
    Frame::setVisible(visibility);
}

void Window::setFullscreen(bool goFullscreen) const {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowMonitor(this->window, goFullscreen ? glfwGetPrimaryMonitor() : nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
}

void Window::setIcon(const std::string& identifier) const {
    GLFWimage images[1];
    int iconWidth, iconHeight, bitsPerPixel;
    Image icon(FilesystemResourceProvider::getResourceAbsolutePath(identifier), &iconWidth, &iconHeight, &bitsPerPixel, 4, false);
    runtime_assert(icon.getData(), TR("error.engine.icon_has_no_data"));
    images[0].width = iconWidth;
    images[0].height = iconHeight;
    images[0].pixels = icon.getData();
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
    plane.setMaterial(Resource::getResource<MaterialTextured>("file://materials/splashscreen.json").castAssert<MaterialBase>());
    plane.render(glm::identity<glm::mat4>());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    this->surface.render(glm::identity<glm::mat4>());
    glfwSwapBuffers(this->window);
}
