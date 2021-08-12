#include "engine.h"

#if __has_include(<windows.h>)
#include <windows.h>
#endif

#include <iostream>

#define IMGUI_USER_CONFIG "../../src/config/imguiConfig.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../loader/jsonSettingsLoader.h"
#include "../loader/image.h"
#include "../sound/alSoundManager.h"

engine::engine(const std::string& configPath) {
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

    if (this->settingsLoader->hasValue("engine", "maxPointLights")) {
        int maxLights;
        this->settingsLoader->getValue("engine", "maxPointLights", &maxLights);
        shaderFile::addPreprocessorSymbol("MAX_POINT_LIGHTS", std::to_string(maxLights));
    }
    if (this->settingsLoader->hasValue("engine", "maxPointLights")) {
        int maxLights;
        this->settingsLoader->getValue("engine", "maxDirectionalLights", &maxLights);
        shaderFile::addPreprocessorSymbol("MAX_DIRECTIONAL_LIGHTS", std::to_string(maxLights));
    }
    if (this->settingsLoader->hasValue("engine", "maxSpotLights")) {
        int maxLights;
        this->settingsLoader->getValue("engine", "maxSpotLights", &maxLights);
        shaderFile::addPreprocessorSymbol("MAX_SPOT_LIGHTS", std::to_string(maxLights));
    }
    for (const auto& [name, object] : engine::shaders) {
        object->compile();
    }

    for (const auto& [name, object] : engine::textures) {
        object->compile();
    }
    for (const auto& [name, object] : engine::materials) {
        object->compile();
    }

    this->callRegisteredFunctions(&(this->initFunctions));

    for (const auto& [name, scriptProvider] : this->scriptProviders) {
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
}

void engine::run() {
    this->lastTime = this->currentTime;
    this->currentTime = glfwGetTime();

    while (!glfwWindowShouldClose(this->window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->render();
        this->soundManager->setListenerPosition(this->getWorld()->getCamera()->getPosition());
        this->soundManager->setListenerRotation(this->getWorld()->getCamera()->getRotation(), this->getWorld()->getCamera()->getUpVector());
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
    for (const auto& [name, shader] : engine::shaders) {
        shader->setUniform("p", this->getWorld()->getCamera()->getProjectionMatrix());
        shader->setUniform("v", this->getWorld()->getCamera()->getViewMatrix());
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    callRegisteredFunctions(&(this->renderFunctions));

#if DEBUG
    if (this->getConsole()->getEnabled()) {
        this->getConsole()->render();
    }
#endif

    for (const auto& [name, scriptProvider] : this->scriptProviders) {
        scriptProvider->render(this->getDeltaTime());
    }

    this->worldPtr->render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void engine::stop() {
    engine::logInfoImportant("ChiraEngine", "Gracefully exiting...");

    for (const auto& [name, scriptProvider] : this->scriptProviders) {
        scriptProvider->stop();
    }

    for (const auto& [name, object] : engine::textures) {
        object->discard();
    }
    for (const auto& [name, object] : engine::shaders) {
        object->discard();
    }

    callRegisteredFunctions(&(this->stopFunctions));

    this->worldPtr->discard();
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
    engine::shaders.insert(std::make_pair(name, s));
}

shader* engine::getShader(const std::string& name) {
    if (engine::shaders.count(name) == 0) {
        engine::logError("engine::getShader", "Shader " + name + " is not recognized, check that you registered it properly");
    }
    return engine::shaders.at(name).get();
}

void engine::addTexture(const std::string& name, texture* t) {
    engine::textures.insert(std::make_pair(name, t));
}

texture* engine::getTexture(const std::string& name) {
    if (engine::textures.count(name) == 0) {
        engine::logError("engine::getTexture", "Texture " + name + " is not recognized, check that you registered it properly");
    }
    return engine::textures.at(name).get();
}

void engine::addMesh(const std::string& name, mesh* m) {
    engine::meshes.insert(std::make_pair(name, m));
}

mesh* engine::getMesh(const std::string& name) {
    if (engine::meshes.count(name) == 0) {
        engine::logError("engine::getMesh", "Mesh " + name + " is not recognized, check that you registered it properly");
    }
    return engine::meshes.at(name).get();
}

void engine::addMaterial(const std::string& name, abstractMaterial* m) {
    engine::materials.insert(std::make_pair(name, m));
}

abstractMaterial* engine::getMaterial(const std::string& name) {
    if (engine::materials.count(name) == 0) {
        engine::logError("engine::getMaterial", "Material " + name + " is not recognized, check that you registered it properly");
    }
    return engine::materials.at(name).get();
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

world* engine::getWorld() {
    if (!this->worldPtr) {
        engine::logWarning("engine::getWorld", "Must set world in engine::setWorld for this call to function");
        return nullptr;
    }
    return this->worldPtr.get();
}

void engine::setWorld(class world* newWorld) {
    this->worldPtr.reset(newWorld);
    this->worldPtr->compile();
}

void engine::setSettingsLoaderDefaults() {
    this->settingsLoader->load();
    this->settingsLoader->addCategory("engine");
    this->settingsLoader->setValue("engine", "iconPath", std::string("ui/icon.png"), false, false);
    this->settingsLoader->setValue("engine", "title", std::string("ChiraEngine"), false, false);
    this->settingsLoader->setValue("engine", "maxPointLights", 64, false, false);
    this->settingsLoader->setValue("engine", "maxDirectionalLights", 4, false, false);
    this->settingsLoader->setValue("engine", "maxSpotLights", 4, false, false);
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

const GLFWwindow* engine::getWindow() const {
    return this->window;
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
    engine::chiraLogger.logInfo(source, message);
    engine::runLogHooks(INFO, source, message);
}

void engine::logInfoImportant(const std::string& source, const std::string& message) {
    engine::chiraLogger.logInfoImportant(source, message);
    engine::runLogHooks(INFO_IMPORTANT, source, message);
}

void engine::logOutput(const std::string& source, const std::string& message) {
    engine::chiraLogger.logOutput(source, message);
    engine::runLogHooks(OUTPUT, source, message);
}

void engine::logWarning(const std::string& source, const std::string& message) {
    engine::chiraLogger.logWarning(source, message);
    engine::runLogHooks(WARN, source, message);
}

void engine::logError(const std::string& source, const std::string& message) {
    engine::chiraLogger.logError(source, message);
    engine::runLogHooks(ERR, source, message);
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
