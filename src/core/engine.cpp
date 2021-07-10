#include "engine.h"

#include <cassert>
#if __has_include(<windows.h>)
#include <windows.h>
#endif

static void debugPrint(const std::string &string) {
#if DEBUG
    std::cout << string << std::endl;
#endif
}

void engine::errorCallback(int error, const char* description) {
    std::fprintf(stderr, "Error %d: %s\n", error, description);
}

void engine::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void engine::processInput(GLFWwindow* inputWindow) {
    for (const keybind& i : this->inputManager.keybinds) {
        if (glfwGetKey(inputWindow, i.getButton()) == i.getAction()) {
            i.fire();
        }
    }
}

void engine::init() {
    // compile shaders, add textures, etc.
}

void engine::start(const std::string& iconPath) {
    this->started = true;
#ifdef WIN32
#if RELEASE
    FreeConsole();
#endif
#endif

    if (!glfwInit()) {
        std::cerr << "Error: GLFW not defined" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(this->errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    this->window = glfwCreateWindow(1280, 720, "Basic Game Engine", nullptr, nullptr);
    if (!this->window) {
        std::cerr << "Error: Window creation failed" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(this->window);
    if (!iconPath.empty()) {
        // todo: load icon from a settings module rather than an argument
        this->setIcon(iconPath);
    }

#if DEBUG
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    printf("Using GLFW v%d.%d.%d \n", major, minor, rev);
#endif

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Error: OpenGL 3.3 Core must be available to run this program" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    int width, height;
    glfwGetFramebufferSize(this->window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(this->window, this->framebufferSizeCallback);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glfwSwapInterval(1);

    this->init();

    while (!glfwWindowShouldClose(this->window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->processInput(this->window);

        this->render();

        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

    this->stop();
}

void engine::render() {
    // Empty now
}

void engine::stop() const {
    debugPrint("Gracefully exiting...");
    glfwDestroyWindow(this->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void engine::setIcon(const std::string& iconPath) {
#if DEBUG
    assert(this->started);
#endif
    GLFWimage images[1];
    int width, height, bitsPerPixel;
    image icon(iconPath, &width, &height, &bitsPerPixel, 4);
#if DEBUG
    assert(icon.getData());
#endif
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = icon.getData();
    glfwSetWindowIcon(this->window, 1, images);
}

void engine::setInputManager(input* newInput) {
    this->inputManager = *newInput;
}

input* engine::getInputManager() {
    return &(this->inputManager);
}
