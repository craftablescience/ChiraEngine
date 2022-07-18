#include "Window.h"

#include <core/Engine.h>
#include <config/Config.h>
#include <event/Events.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include <loader/image/Image.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <render/material/MaterialFramebuffer.h>

using namespace chira;

bool Window::createGLFWWindow(std::string_view title) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
    glfwWindowHint(GLFW_VISIBLE, this->visible ? GLFW_TRUE : GLFW_FALSE);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (this->fullscreen) {
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    }
    this->window = glfwCreateWindow(this->width, this->height, title.data(), nullptr, nullptr);
    if (!this->window) {
        Logger::log(LOG_ERROR, "GLFW", TR("error.glfw.window"));
        return false;
    }
    if (this->fullscreen)
        glfwSetWindowMonitor(this->window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    else {
        bool startMaximized = true;
        Engine::getSettings()->getValue("graphics", "startMaximized", &startMaximized);
        if (startMaximized)
            glfwMaximizeWindow(this->window);
    }

    glfwSetWindowUserPointer(this->window, this);
    glfwMakeContextCurrent(this->window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        Logger::log(LOG_ERROR, "OpenGL", TRF("error.opengl.version", GL_VERSION_STRING_PRETTY));
        return false;
    }
    glfwSwapInterval(1);

    this->setIcon("file://textures/ui/icon.png");

    glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(this->window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    bool rawMouseMotion = false;
    Engine::getSettings()->getValue("input", "rawMouseMotion", &rawMouseMotion);
    if (glfwRawMouseMotionSupported() && rawMouseMotion)
        glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* w, int width_, int height_) {
        if (auto window_ = static_cast<Window*>(glfwGetWindowUserPointer(w)))
            window_->setFrameSize({width_, height_});
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

    return true;
}

static void makeSurface(Window* window, MeshDataBuilder* surface) {
    surface->addSquare({}, {2, -2}, SignedAxis::ZN, 0);
    surface->setMaterial(Resource::getResource<MaterialFramebuffer>("file://materials/window.json", window).castAssert<MaterialBase>());
}

Window::Window(std::string name_, std::string_view title, int width_, int height_, bool fullscreen_, ColorRGB backgroundColor_, bool smoothResize, bool startVisible)
    : Frame(std::move(name_), width_, height_, backgroundColor_, smoothResize, false)
    , fullscreen(fullscreen_) {
    this->visible = startVisible;
    if (this->createGLFWWindow(title)) {
        this->createFramebuffer();
        makeSurface(this, &this->surface);
    }
}

Window::Window(std::string_view title, int width_, int height_, bool fullscreen_, ColorRGB backgroundColor_, bool smoothResize, bool startVisible)
    : Frame(width_, height_, backgroundColor_, smoothResize, false)
    , fullscreen(fullscreen_) {
    this->visible = startVisible;
    if (this->createGLFWWindow(title)) {
        this->createFramebuffer();
        makeSurface(this, &this->surface);
    }
}

void Window::update(glm::mat4 /*parentTransform*/) {
    glfwMakeContextCurrent(this->window);

    Frame::update(this->fboHandle, this->width, this->height);

    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    this->surface.render(glm::identity<glm::mat4>());
    glfwSwapBuffers(this->window);
}

Window::~Window() {
    glfwDestroyWindow(this->window);
}

const Window* Window::getWindow() const {
    return this;
}

Window* Window::getWindow() {
    return this;
}

void Window::setFrameSize(glm::vec2i newSize) {
    Frame::setFrameSize(newSize);
    glfwSetWindowSize(this->window, this->width, this->height);
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
    } else {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
