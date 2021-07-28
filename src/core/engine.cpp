#include "engine.h"

#include <iostream>
#if __has_include(<windows.h>)
#include <windows.h>
#endif

#define IMGUI_USER_CONFIG "../../src/config/imguiConfig.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../loader/jsonSettingsLoader.h"
#include "../sound/alSoundManager.h"

logger engine::logger{};
std::vector<std::function<void(const loggerType, const std::string&, const std::string&)>> engine::loggerFunctions{};

engine::engine(const std::string& configPath) : scriptProviders() {
#ifdef WIN32
#if DEBUG
    system(("chcp " + std::to_string(CP_UTF8) + " > nul").c_str());
#else
    FreeConsole();
#endif
#endif
    virtualFileSystem::addResourceDirectory(ENGINE_FILESYSTEM_PREFIX);
    this->setSettingsLoader(new jsonSettingsLoader(configPath));
    this->lastTime = 0;
    this->currentTime = 0;
    this->lastMouseX = -1;
    this->lastMouseY = -1;
}

void engine::errorCallback(int error, const char* description) {
#if DEBUG
    std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
#endif
}

void engine::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void engine::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* e = static_cast<engine*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_REPEAT) return;
    for (keybind& k : *e->getKeybinds()) {
        if (k.getButton() == key && k.getAction() == action) {
            k.run(e);
        }
    }
}

void engine::keyboardRepeatingCallback() {
    for (keybind& k : this->keybinds) {
        if (glfwGetKey(this->window, k.getButton()) && k.getAction() == GLFW_REPEAT) {
            k.run(this);
        }
    }
}

void engine::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* e = static_cast<engine*>(glfwGetWindowUserPointer(window));
    for (keybind& k : *e->getKeybinds()) {
        if (k.getButton() == button && k.getAction() == action) {
            k.run(e);
        }
    }
}

void engine::mouseButtonRepeatingCallback() {
    for (keybind& k : this->keybinds) {
        if (k.isMouse() && (glfwGetMouseButton(this->window, k.getButton()) && k.getAction() == GLFW_REPEAT)) {
            k.run(this);
        }
    }
}

void engine::mouseMovementCallback(GLFWwindow* window, double xPos, double yPos) {
    auto* e = static_cast<engine*>(glfwGetWindowUserPointer(window));

    if (e->lastMouseX == -1) e->lastMouseX = xPos;
    if (e->lastMouseY == -1) e->lastMouseY = yPos;

    int width, height;
    glfwGetWindowSize(e->window, &width, &height);
    double xOffset = xPos - e->lastMouseX;
    double yOffset = yPos - e->lastMouseY;

    for (mousebind& bind : *e->getMousebinds()) {
        if (bind.getType() == MOVE) {
            bind.run(e, xOffset, yOffset);
        }
    }

    e->lastMouseX = xPos;
    e->lastMouseY = yPos;
}

void engine::mouseScrollCallback(GLFWwindow* window, double xPos, double yPos) {
    auto* e = static_cast<engine*>(glfwGetWindowUserPointer(window));
    for (mousebind& bind : *e->getMousebinds()) {
        if (bind.getType() == SCROLL) {
            bind.run(e, xPos, yPos);
        }
    }
}

void engine::init() {
    this->started = true;

    engine::addLogHook([this](const loggerType type, const std::string& source, const std::string& message) {
        this->getConsole()->engineLoggingHook(type, source, message);
    });

    bool angelscriptEnabled = true;
    this->settingsLoader->getValue("scripting", "angelscript", &angelscriptEnabled);
    if (angelscriptEnabled) {
        this->addScriptProvider("angelscript", new angelscriptProvider{});
    }

    if (!glfwInit()) {
        engine::logError("GLFW", "GLFW not defined");
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(this->errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int windowWidth = 1600;
    this->settingsLoader->getValue("graphics", "windowWidth", &windowWidth);
    int windowHeight = 900;
    this->settingsLoader->getValue("graphics", "windowHeight", &windowHeight);
    bool fullscreen = false;
    this->settingsLoader->getValue("graphics", "fullscreen", &fullscreen);
    std::string title = "Basic Game Engine";
    this->settingsLoader->getValue("engine", "title", &title);
    this->window = glfwCreateWindow(windowWidth,
                                    windowHeight,
                                    title.c_str(),
                                    fullscreen ? glfwGetPrimaryMonitor() : nullptr,
                                    nullptr);
    if (!this->window) {
        engine::logError("GLFW", "Window creation failed");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(this->window);
    glfwSetWindowUserPointer(this->window, this);

    if (this->settingsLoader->hasValue("engine", "iconPath")) {
        std::string path{};
        this->settingsLoader->getValue("engine", "iconPath", &path);
        this->setIcon(path);
    } else {
        engine::logWarning("ChiraEngine", "You should not unset the iconPath property unless you are a trained professional!");
    }

#if DEBUG
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    engine::logInfo("GLFW", "Using GLFW v" + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(rev));
#endif

    if (!gladLoadGL(glfwGetProcAddress)) {
        engine::logError("OpenGL", "OpenGL 3.3 Core must be available to run this program");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

#if DEBUG
    int vertexAttributes, textureUnits;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &vertexAttributes);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    engine::logInfo("OpenGL", "Maximum number of vertex attributes is " + std::to_string(vertexAttributes));
    engine::logInfo("OpenGL", "Maximum number of texture units is " + std::to_string(textureUnits));
#endif

    int width, height;
    glfwGetFramebufferSize(this->window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(this->window, this->framebufferSizeCallback);
    engine::setBackgroundColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);

    glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(this->window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    bool rawMouseMotion = false;
    this->settingsLoader->getValue("input", "rawMouseMotion", &rawMouseMotion);
    if (glfwRawMouseMotionSupported() && rawMouseMotion) {
        glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwSetKeyCallback(this->window, keyboardCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(this->window, mouseMovementCallback);
    glfwSetScrollCallback(this->window, mouseScrollCallback);

#if DEBUG
    IMGUI_CHECKVERSION();
#endif
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    engine::logInfo("ImGUI", "ImGUI loaded successfully");

    bool openalEnabled = true;
    this->settingsLoader->getValue("scripting", "angelscript", &openalEnabled);
    if (openalEnabled) {
        this->setSoundManager(new alSoundManager{});
    }
    this->soundManager->init();

    for (auto const& [name, object] : this->compilableObjects) {
        object.get()->compile();
    }
    this->callRegisteredFunctions(&(this->initFunctions));

    for (auto const& [name, scriptProvider] : this->scriptProviders) {
        scriptProvider->initProvider();

        if (name == "angelscript") {
            auto* angel = (angelscriptProvider*) scriptProvider.get();

            angel->registerGlobalFunction(engine::setBackgroundColor, "setBackgroundColor");

            angel->asEngine->RegisterGlobalFunction("void captureMouse(bool)", asMETHOD(engine, captureMouse), asCALL_THISCALL_ASGLOBAL, this);
            angel->asEngine->RegisterGlobalFunction("bool isMouseCaptured()", asMETHOD(engine, isMouseCaptured), asCALL_THISCALL_ASGLOBAL, this);
            angel->asEngine->RegisterGlobalFunction("void showConsole(bool)", asMETHOD(engine, showConsole), asCALL_THISCALL_ASGLOBAL, this);
        }

        scriptProvider->initScripts();
    }

    this->getCamera()->init(this);
}

void engine::run() {
    this->lastTime = this->currentTime;
    this->currentTime = glfwGetTime();

    while (!glfwWindowShouldClose(this->window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->render();
        this->soundManager->setListenerPosition(this->getCamera()->getPosition());
        this->soundManager->setListenerRotation(this->getCamera()->getRotation(), this->getCamera()->getUpVector());
        this->soundManager->update();
        glfwSwapBuffers(this->window);
        glfwPollEvents();
        this->keyboardRepeatingCallback();
        this->mouseButtonRepeatingCallback();
    }

    this->stop();
}

void engine::render() {
    this->lastTime = this->currentTime;
    this->currentTime = glfwGetTime();
    for (auto const& [name, object] : this->compilableObjects) {
        if (name.rfind("shader", 0) == 0) {
            dynamic_cast<shader*>(object.get())->setUniform("p", this->getCamera()->getProjectionMatrix());
            dynamic_cast<shader*>(object.get())->setUniform("v", this->getCamera()->getViewMatrix());
        }
    }
    callRegisteredFunctions(&(this->renderFunctions));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

#if DEBUG
    if (this->getConsole()->getEnabled()) {
        this->getConsole()->render();
    }
#endif

    for (auto const& [name, scriptProvider] : this->scriptProviders) {
        scriptProvider->render(this->getDeltaTime());
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void engine::stop() {
    engine::logInfoImportant("ChiraEngine", "Gracefully exiting...");

    for (auto const& [name, scriptProvider] : this->scriptProviders) {
        scriptProvider->stop();
    }
    for (auto const& [name, object] : this->compilableObjects) {
        object->discard();
    }
    callRegisteredFunctions(&(this->stopFunctions));

    this->soundManager->stop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(this->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void engine::setBackgroundColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void engine::addKeybind(const keybind& keybind) {
    this->keybinds.push_back(keybind);
}

std::vector<keybind>* engine::getKeybinds() {
    return &(this->keybinds);
}

void engine::addMousebind(const mousebind& mousebind) {
    this->mousebinds.push_back(mousebind);
}

std::vector<mousebind>* engine::getMousebinds() {
    return &(this->mousebinds);
}

void engine::addShader(const std::string& name, shader* s) {
    this->compilableObjects.insert(std::make_pair("shaders/" + name, s));
}

shader* engine::getShader(const std::string& name) {
    if (this->compilableObjects.count("shaders/" + name) == 0) {
        engine::logError("engine::getShader", "Shader " + name + " is not recognized, check that you registered it properly");
    }
    return (shader*) this->compilableObjects.at("shaders/" + name).get();
}

void engine::addTexture(const std::string& name, texture* t) {
    this->compilableObjects.insert(std::make_pair("textures/" + name, t));
}

texture* engine::getTexture(const std::string& name) {
    if (this->compilableObjects.count("textures/" + name) == 0) {
        engine::logError("engine::getTexture", "Texture " + name + " is not recognized, check that you registered it properly");
    }
    return (texture*) this->compilableObjects.at("textures/" + name).get();
}

void engine::addMesh(const std::string& name, mesh* m) {
    this->compilableObjects.insert(std::make_pair("meshes/" + name, m));
}

mesh* engine::getMesh(const std::string& name) {
    if (this->compilableObjects.count("meshes/" + name) == 0) {
        engine::logError("engine::getMesh", "Mesh " + name + " is not recognized, check that you registered it properly");
    }
    return (mesh*) this->compilableObjects.at("meshes/" + name).get();
}

void engine::addScriptProvider(const std::string& name, abstractScriptProvider* scriptProvider) {
    this->scriptProviders.insert(std::make_pair(name, scriptProvider));
}

abstractScriptProvider* engine::getScriptProvider(const std::string& name) {
    if (this->scriptProviders.count(name) == 0) {
        engine::logError("engine::getScriptProvider", "Script provider " + name + " is not recognized, check that you registered it properly");
    }
    return this->scriptProviders.at(name).get();
}

void engine::setSoundManager(abstractSoundManager* newSoundManager) {
    this->soundManager.reset(newSoundManager);
}

abstractSoundManager* engine::getSoundManager() {
    if (!this->soundManager) {
        engine::logWarning("engine::getSoundManager", "Must set sound manager in engine::setSoundManager for this call to function");
        return nullptr;
    }
    return this->soundManager.get();
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

abstractCamera* engine::getCamera() const {
    if (!this->camera.get()) {
        engine::logWarning("engine::getCamera", "Must set camera in engine::setCamera for this call to function");
        return nullptr;
    }
    return this->camera.get();
}

void engine::setCamera(abstractCamera* newCamera) {
    if (this->camera.get()) {
        this->camera->setCurrent(false);
    }
    this->camera.reset(newCamera);
    this->camera->setCurrent(true);
}

abstractSettingsLoader* engine::getSettingsLoader() {
    if (!this->settingsLoader) {
        engine::logWarning("engine::getSettingsLoader", "Must set settings loader in engine::setSettingsLoader for this call to function");
        return nullptr;
    }
    return this->settingsLoader.get();
}

void engine::setSettingsLoader(abstractSettingsLoader* newSettingsLoader) {
    this->settingsLoader.reset(newSettingsLoader);
    this->setSettingsLoaderDefaults();
}

void engine::setSettingsLoaderDefaults() {
    this->settingsLoader->load();
    this->settingsLoader->addCategory("engine");
    this->settingsLoader->setValue("engine", "iconPath", std::string("ui/icon.png"), false, false);
    this->settingsLoader->setValue("engine", "title", std::string("Basic Game Engine"), false, false);
    this->settingsLoader->addCategory("audio");
    this->settingsLoader->setValue("audio", "openal", true, false, false);
    this->settingsLoader->addCategory("input");
    this->settingsLoader->setValue("input", "rawMouseMotion", true, false, false);
    this->settingsLoader->setValue("input", "invertYAxis", false, false, false);
    this->settingsLoader->addCategory("graphics");
    this->settingsLoader->setValue("graphics", "windowWidth", 1600, false, false);
    this->settingsLoader->setValue("graphics", "windowHeight", 900, false, false);
    this->settingsLoader->setValue("graphics", "fullscreen", false, false, false);
    this->settingsLoader->addCategory("scripting");
    this->settingsLoader->setValue("scripting", "angelscript", true, false, false);
    this->settingsLoader->save();
}

void engine::callRegisteredFunctions(const std::vector<std::function<void(engine*)>>* list) {
    for (const std::function<void(engine*)>& func : *list) {
        func(this);
    }
}

bool engine::isStarted() const {
    return this->started;
}

double engine::getDeltaTime() const {
    return this->currentTime - this->lastTime;
}

void engine::setIcon(const std::string& iconPath) {
#if DEBUG
    assert(this->started);
#endif
    GLFWimage images[1];
    int width, height, bitsPerPixel;
    image icon(virtualFileSystem::getTexturePath(iconPath), &width, &height, &bitsPerPixel, 4, false);
#if DEBUG
    assert(icon.getData());
#endif
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = icon.getData();
    glfwSetWindowIcon(this->window, 1, images);
}

void engine::logInfo(const std::string& source, const std::string& message) {
    engine::logger.logInfo(source, message);
    engine::runLogHooks(INFO, source, message);
}

void engine::logInfoImportant(const std::string& source, const std::string& message) {
    engine::logger.logInfoImportant(source, message);
    engine::runLogHooks(INFO_IMPORTANT, source, message);
}

void engine::logOutput(const std::string& source, const std::string& message) {
    engine::logger.logOutput(source, message);
    engine::runLogHooks(OUTPUT, source, message);
}

void engine::logWarning(const std::string& source, const std::string& message) {
    engine::logger.logWarning(source, message);
    engine::runLogHooks(WARNING, source, message);
}

void engine::logError(const std::string& source, const std::string& message) {
    engine::logger.logError(source, message);
    engine::runLogHooks((loggerType) 4, source, message);
}

void engine::addLogHook(const std::function<void(const loggerType,const std::string&,const std::string&)>& function) {
    engine::loggerFunctions.push_back(function);
}

void engine::runLogHooks(const loggerType type, const std::string& source, const std::string& message) {
    for (const std::function<void(const loggerType,const std::string&,const std::string&)>& function : engine::loggerFunctions) {
        function(type, source, message);
    }
}

void engine::captureMouse(bool capture) {
    this->mouseCaptured = capture;
    if (capture) {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    } else {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
}

bool engine::isMouseCaptured() const {
    return this->mouseCaptured;
}

void engine::showConsole(bool shouldShow) {
    this->getConsole()->setEnabled(shouldShow);
}

console* engine::getConsole() {
    return &(this->consoleUI);
}
