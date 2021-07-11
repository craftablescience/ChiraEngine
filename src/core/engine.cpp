#include "engine.h"

#include <iostream>
#include "../loader/image.h"

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
    for (const keybind& i : this->keybinds) {
        if (glfwGetKey(inputWindow, i.getButton()) == i.getAction()) {
            i.fire(this);
        }
    }
}

void engine::init(const std::string& iconPath) {
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

#if DEBUG
int vertexAttributes, textureUnits;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &vertexAttributes);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    std::cout << "OpenGL: Maximum number of vertex attributes is " << vertexAttributes << std::endl;
    std::cout << "OpenGL: Maximum number of texture units is " << textureUnits << std::endl;
#endif

    int width, height;
    glfwGetFramebufferSize(this->window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(this->window, this->framebufferSizeCallback);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);

    for (auto const& [name, object] : this->glObjects) {
        object.get()->compile();
    }
    callRegisteredFunctions(&(this->initFunctions));
}

void engine::run() {
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
    callRegisteredFunctions(&(this->renderFunctions));
}

void engine::stop() {
    debugPrint("Gracefully exiting...");
    for (auto const& [name, object] : this->glObjects) {
        object->discard();
    }
    callRegisteredFunctions(&(this->stopFunctions));
    glfwDestroyWindow(this->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void engine::addKeybind(const keybind& keybind) {
    this->keybinds.push_back(keybind);
}

void engine::addShader(const std::string& name, shader* s) {
    this->glObjects.insert(std::make_pair("shaders/" + name, s));
}

shader* engine::getShader(const std::string& name) {
    return (shader*) this->glObjects.at("shaders/" + name).get();
}

void engine::addTexture(const std::string& name, texture* t) {
    this->glObjects.insert(std::make_pair("textures/" + name, t));
}

texture* engine::getTexture(const std::string& name) {
    return (texture*) this->glObjects.at("textures/" + name).get();
}

void engine::addMesh(const std::string& name, mesh* m) {
    this->glObjects.insert(std::make_pair("meshes/" + name, m));
}

mesh* engine::getMesh(const std::string& name) {
    return (mesh*) this->glObjects.at("meshes/" + name).get();
}

void engine::addInitFunction(const std::function<void(engine*)>& init) {
    this->initFunctions.push_back(init);
}

void engine::addRenderFunction(const std::function<void(engine*)>& render) {
    this->renderFunctions.push_back(render);
}

void engine::addStopFunction(const std::function<void(engine*)>& stop) {
    this->stopFunctions.push_back(stop);
}

void engine::setIcon(const std::string& iconPath) {
#if DEBUG
    assert(this->started);
#endif
    GLFWimage images[1];
    int width, height, bitsPerPixel;
    image icon(iconPath, &width, &height, &bitsPerPixel, 4, false);
#if DEBUG
    assert(icon.getData());
#endif
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = icon.getData();
    glfwSetWindowIcon(this->window, 1, images);
}

void engine::callRegisteredFunctions(const std::vector<std::function<void(engine*)>> *list) {
    for (const std::function<void(engine*)>& func : *list) {
        func(this);
    }
}

bool engine::isStarted() const {
    return this->started;
}
