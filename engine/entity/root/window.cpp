#include "window.h"

#include <core/engine.h>
#include <config/glVersion.h>
#include <event/events.h>
#include <i18n/translationManager.h>
#include <input/inputManager.h>
#include <loader/image/image.h>
#include <resource/provider/filesystemResourceProvider.h>
#include <render/material/materialFramebuffer.h>

using namespace chira;

bool Window::createGLFWWindow() {
    int windowWidth = 1600;
    Engine::getSettingsLoader()->getValue("graphics", "windowWidth", &windowWidth);
    int windowHeight = 900;
    Engine::getSettingsLoader()->getValue("graphics", "windowHeight", &windowHeight);
    bool fullscreen = false;
    Engine::getSettingsLoader()->getValue("graphics", "fullscreen", &fullscreen);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
    this->window = glfwCreateWindow(windowWidth, windowHeight, TRC("ui.window.title"), nullptr, nullptr);
    if (!this->window) {
        Logger::log(LogType::ERROR, "GLFW", TR("error.glfw.window"));
        return false;
    }
    if (fullscreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwSetWindowMonitor(this->window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        bool startMaximized = true;
        Engine::getSettingsLoader()->getValue("graphics", "startMaximized", &startMaximized);
        if (startMaximized)
            glfwMaximizeWindow(this->window);
    }

    glfwSetWindowUserPointer(this->window, this);
    glfwMakeContextCurrent(this->window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        Logger::log(LogType::ERROR, "OpenGL", fmt::format("error.opengl.version", GL_VERSION_STRING_PRETTY));
        return false;
    }
    glfwSwapInterval(1);

    this->setIcon("file://textures/ui/icon.png");

    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* w, int width, int height) {
        if (auto window_ = static_cast<Window*>(glfwGetWindowUserPointer(w)))
            window_->setFrameSize({width, height});
    });

    glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(this->window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    bool rawMouseMotion = false;
    Engine::getSettingsLoader()->getValue("input", "rawMouseMotion", &rawMouseMotion);
    if (glfwRawMouseMotionSupported() && rawMouseMotion)
        glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetKeyCallback(this->window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        if (action == GLFW_REPEAT) return;
        for (auto& keybind : InputManager::getKeyButtonCallbacks()) {
            if (keybind.getKey() == static_cast<Key>(key) && keybind.getEventType() == static_cast<InputKeyEventType>(action))
                keybind();
        }
    });
    glfwSetMouseButtonCallback(this->window, [](GLFWwindow* w, int key, int action, int mods) {
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
    glfwSetScrollCallback(this->window, [](GLFWwindow* w, double x, double y) {
        for (auto& movebind : InputManager::getMouseMovementCallbacks()) {
            if (movebind.getType() == InputMouseMovementEventType::SCROLL)
                movebind(x,y);
        }
    });
    glfwSetWindowIconifyCallback(this->window, [](GLFWwindow* w, int isIconified) {
        static_cast<Window*>(glfwGetWindowUserPointer(w))->iconified = (isIconified == GLFW_TRUE);
    });
    glfwSetDropCallback(this->window, [](GLFWwindow* w, int count, const char** paths) {
        std::vector<std::string> files;
        files.reserve(count);
        for (int i = 0; i < count; i++)
            files.emplace_back(paths[i]);
        Events::createEvent("chira::engine::files_dropped", files);
    });

    return true;
}

static void makeSurface(Window* window, MeshDataBuilder* surface) {
    surface->enableBackfaceCulling(false);
    surface->addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    surface->setMaterial(Resource::getResource<MaterialFramebuffer>("file://materials/window.json", window).castAssert<MaterialBase>());
}

Window::Window(const std::string& name_, const std::string& title_, int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Frame(name_, width_, height_, backgroundColor_, smoothResize, false) {
    this->createGLFWWindow();
    this->createFramebuffer();
    makeSurface(this, &this->surface);
}

Window::Window(const std::string& title_, int width_, int height_, ColorRGB backgroundColor_, bool smoothResize)
    : Frame(width_, height_, backgroundColor_, smoothResize, false) {
    this->createGLFWWindow();
    this->createFramebuffer();
    makeSurface(this, &this->surface);
}

void Window::render(glm::mat4 parentTransform) {
    Frame::render(std::forward<glm::mat4>(parentTransform), 0, this->width, this->height);
    glDisable(GL_DEPTH_TEST);
    this->surface.render(glm::identity<glm::mat4>());
}

Window::~Window() {
    //todo(viewport): de-init GLFW and ImGUI
    glfwDestroyWindow(this->window);
}

const Window* Window::getWindow() const {
    return this;
}

Window* Window::getWindow() {
    return this;
}

void Window::setFrameSize(glm::vec<2, int> newSize) {
    Frame::setFrameSize(newSize);
    glfwSetWindowSize(this->window, this->width, this->height);
}

glm::vec<2, double> Window::getMousePosition() const {
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

void Window::setIcon(const std::string& identifier) const {
    GLFWimage images[1];
    int width, height, bitsPerPixel;
    Image icon(FilesystemResourceProvider::getResourceAbsolutePath(identifier), &width, &height, &bitsPerPixel, 4, false);
    chira_assert(icon.getData(), TR("error.engine.icon_has_no_data"));
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = icon.getData();
    glfwSetWindowIcon(this->window, 1, images);
}

void Window::shouldStopAfterThisFrame(bool yes) const {
    glfwSetWindowShouldClose(this->window, yes ? GLFW_TRUE : GLFW_FALSE);
}

void Window::displaySplashScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    MeshDataBuilder plane;
    plane.addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    plane.setMaterial(Resource::getResource<MaterialTextured>("file://materials/splashscreen.json").castAssert<MaterialBase>());
    plane.render(glm::identity<glm::mat4>());
}
