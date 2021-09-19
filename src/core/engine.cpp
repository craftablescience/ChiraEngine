#include "engine.h"

#define IMGUI_USER_CONFIG "../../src/config/imguiConfig.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "fmt/core.h"
#include <iostream>
#include "../config/glVersion.h"
#include "../loader/jsonSettingsLoader.h"
#include "../loader/image.h"
#include "../sound/alSoundManager.h"
#include "../implementation/discordRichPresence.h"
#include "../resource/filesystemResourceProvider.h"
#include "../i18n/translationManager.h"
#include "../resource/fontResource.h"

#if __has_include(<windows.h>)
#include <windows.h>
#endif

const std::string ENGINE_FILESYSTEM_PATH = "resources/engine";

engine::engine(const std::string& configPath) {
#ifdef _WIN32
#if DEBUG
    system(("chcp " + std::to_string(CP_UTF8) + " > nul").c_str());
#else
    FreeConsole();
#endif
#endif
    resourceManager::addResourceProvider("file", new filesystemResourceProvider{"file", ENGINE_FILESYSTEM_PATH});
    engine::setSettingsLoader(new jsonSettingsLoader(configPath));
    // todo: use computer language as default here
    std::string defaultLang = "en";
    engine::getSettingsLoader()->getValue("ui", "language", &defaultLang);
    chira::translationManager::setLanguage(defaultLang);
    chira::translationManager::addTranslationFile("engine");
    this->lastTime = 0;
    this->currentTime = 0;
    this->lastMouseX = -1;
    this->lastMouseY = -1;
}

void engine::errorCallback(int error, const char* description) {
#if DEBUG
    fmt::print(stderr, TR("error.glfw.generic"), error, description);
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

    chira::logger::addCallback([this](const loggerType type, const std::string& source, const std::string& message) {
        this->getConsole()->engineLoggingHook(type, source, message);
    });

    bool angelscriptEnabled = true;
    engine::getSettingsLoader()->getValue("scripting", "angelscript", &angelscriptEnabled);
    if (angelscriptEnabled) {
        this->addScriptProvider("angelscript", new angelscriptProvider{});
    }

    if (!glfwInit()) {
        chira::logger::log(ERR, "GLFW", TR("error.glfw.undefined"));
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(this->errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, chira::GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, chira::GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int windowWidth = 1600;
    engine::getSettingsLoader()->getValue("graphics", "windowWidth", &windowWidth);
    int windowHeight = 900;
    engine::getSettingsLoader()->getValue("graphics", "windowHeight", &windowHeight);
    bool fullscreen = false;
    engine::getSettingsLoader()->getValue("graphics", "fullscreen", &fullscreen);
    this->window = glfwCreateWindow(windowWidth,
                                    windowHeight,
                                    TR("ui.window.title").c_str(),
                                    fullscreen ? glfwGetPrimaryMonitor() : nullptr,
                                    nullptr);
    if (!this->window) {
        chira::logger::log(ERR, "GLFW", TR("error.glfw.window"));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(this->window);
    glfwSetWindowUserPointer(this->window, this);

    if (engine::getSettingsLoader()->hasValue("engine", "iconPath")) {
        std::string path{};
        engine::getSettingsLoader()->getValue("engine", "iconPath", &path);
        this->setIcon(path);
    } else {
        chira::logger::log(WARN, "ChiraEngine", TR("error.engine.unset_icon_path"));
    }

#if DEBUG
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    chira::logger::log(INFO, "GLFW", fmt::format(TR("debug.glfw.version"), major, minor, rev));
#endif

    if (!gladLoadGL(glfwGetProcAddress)) {
        chira::logger::log(ERR, "OpenGL", fmt::format("error.opengl.version", chira::GL_VERSION_STRING_PRETTY));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

#if DEBUG
    int vertexAttributes, textureUnits;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &vertexAttributes);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    chira::logger::log(INFO, "OpenGL", fmt::format(TR("debug.opengl.vertex_attributes"), vertexAttributes));
    chira::logger::log(INFO, "OpenGL", fmt::format(TR("debug.opengl.texture_units"), textureUnits));
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
    engine::getSettingsLoader()->getValue("input", "rawMouseMotion", &rawMouseMotion);
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
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    io.Fonts->Clear();
    ImGui_ImplOpenGL3_Init(chira::GL_VERSION_STRING.data());
    chira::logger::log(INFO, "ImGUI", TR("debug.imgui.success"));

    bool openalEnabled = true;
    engine::getSettingsLoader()->getValue("scripting", "angelscript", &openalEnabled);
    if (openalEnabled) {
        this->setSoundManager(new alSoundManager{});
    }
    this->soundManager->init();

    if (engine::getSettingsLoader()->hasValue("engine", "maxPointLights")) {
        int maxLights;
        engine::getSettingsLoader()->getValue("engine", "maxPointLights", &maxLights);
        shaderResource::addPreprocessorSymbol("MAX_POINT_LIGHTS", std::to_string(maxLights));
    }
    if (engine::getSettingsLoader()->hasValue("engine", "maxPointLights")) {
        int maxLights;
        engine::getSettingsLoader()->getValue("engine", "maxDirectionalLights", &maxLights);
        shaderResource::addPreprocessorSymbol("MAX_DIRECTIONAL_LIGHTS", std::to_string(maxLights));
    }
    if (engine::getSettingsLoader()->hasValue("engine", "maxSpotLights")) {
        int maxLights;
        engine::getSettingsLoader()->getValue("engine", "maxSpotLights", &maxLights);
        shaderResource::addPreprocessorSymbol("MAX_SPOT_LIGHTS", std::to_string(maxLights));
    }

    io.Fonts->AddFontDefault();

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

    resourceManager::precacheResource<fontResource>("file", "fonts/console.json");
    io.Fonts->Build();
}

void engine::run() {
    this->lastTime = this->currentTime;
    this->currentTime = glfwGetTime();

    while (!glfwWindowShouldClose(this->window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->render();
        this->soundManager->setListenerPosition(this->getMainCamera()->getPosition());
        this->soundManager->setListenerRotation(this->getMainCamera()->getRotation(), this->getMainCamera()->getUpVector());
        this->soundManager->update();
        glfwSwapBuffers(this->window);
        glfwPollEvents();
        this->keyboardRepeatingCallback();
        this->mouseButtonRepeatingCallback();
        if (discordRichPresence::initialized()) {
            discordRichPresence::updatePresence();
        }
    }

    this->stop();
}

void engine::render() {
    this->lastTime = this->currentTime;
    this->currentTime = glfwGetTime();

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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void engine::stop() {
    chira::logger::log(INFO_IMPORTANT, "ChiraEngine", TR("debug.engine.exit"));

    for (const auto& [name, scriptProvider] : this->scriptProviders) {
        scriptProvider->stop();
    }

    callRegisteredFunctions(&(this->stopFunctions));

    if (discordRichPresence::initialized()) {
        discordRichPresence::shutdown();
    }

    this->soundManager->stop();

    resourceManager::discardAll();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(this->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
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

void engine::addScriptProvider(const std::string& name, abstractScriptProvider* scriptProvider) {
    this->scriptProviders.insert(std::make_pair(name, scriptProvider));
}

abstractScriptProvider* engine::getScriptProvider(const std::string& name) {
    if (this->scriptProviders.count(name) == 0) {
        chira::logger::log(ERR, "engine::getScriptProvider", fmt::format(TR("error.engine.invalid_script_provider"), name));
    }
    return this->scriptProviders.at(name).get();
}

void engine::setSoundManager(abstractSoundManager* newSoundManager) {
    this->soundManager.reset(newSoundManager);
}

abstractSoundManager* engine::getSoundManager() {
    if (!this->soundManager) {
        chira::logger::log(WARN, "engine::getSoundManager", fmt::format(TR("error.engine.invalid_access"), "sound manager", "engine::setSoundManager"));
        return nullptr;
    }
    return this->soundManager.get();
}

abstractSettingsLoader* engine::getSettingsLoader() {
    if (!engine::settingsLoader) {
        chira::logger::log(WARN, "engine::getSettingsLoader", fmt::format(TR("error.engine.invalid_access"), "settings loader", "engine::setSettingsLoader"));
        return nullptr;
    }
    return engine::settingsLoader.get();
}

void engine::setSettingsLoader(abstractSettingsLoader* newSettingsLoader) {
    engine::settingsLoader.reset(newSettingsLoader);
    engine::setSettingsLoaderDefaults();
}

abstractPhysicsProvider* engine::getPhysicsProvider() {
    if (!engine::physicsProvider) {
        chira::logger::log(WARN, "engine::getPhysicsProvider", fmt::format(TR("error.engine.invalid_access"), "physics provider", "engine::setPhysicsProvider"));
        return nullptr;
    }
    return engine::physicsProvider.get();
}

void engine::setPhysicsProvider(abstractPhysicsProvider* newPhysicsProvider) {
    engine::physicsProvider.reset(newPhysicsProvider);
}

abstractCamera* engine::getMainCamera() const {
    if (!this->mainCamera) {
        chira::logger::log(WARN, "engine::getMainCamera", fmt::format(TR("error.engine.invalid_access"), "main camera", "engine::setMainCamera"));
        return nullptr;
    }
    return this->mainCamera;
}

void engine::setMainCamera(abstractCamera* newCamera) {
    if (this->mainCamera) {
        this->mainCamera->setCurrent(false);
    }
    this->mainCamera = newCamera;
    this->mainCamera->init(this);
    this->mainCamera->setCurrent(true);
}

void engine::setSettingsLoaderDefaults() {
    engine::settingsLoader->load();
    engine::settingsLoader->addCategory("engine");
    engine::settingsLoader->setValue("engine", "iconPath", std::string("textures/ui/icon.png"), false, false);
    engine::settingsLoader->setValue("engine", "consoleColoredText", true, false, false);
    engine::settingsLoader->setValue("engine", "maxPointLights", 64, false, false);
    engine::settingsLoader->setValue("engine", "maxDirectionalLights", 4, false, false);
    engine::settingsLoader->setValue("engine", "maxSpotLights", 4, false, false);
    engine::settingsLoader->addCategory("audio");
    engine::settingsLoader->setValue("audio", "openal", true, false, false);
    engine::settingsLoader->addCategory("graphics");
    engine::settingsLoader->setValue("graphics", "windowWidth", 1600, false, false);
    engine::settingsLoader->setValue("graphics", "windowHeight", 900, false, false);
    engine::settingsLoader->setValue("graphics", "fullscreen", false, false, false);
    engine::settingsLoader->addCategory("input");
    engine::settingsLoader->setValue("input", "rawMouseMotion", true, false, false);
    engine::settingsLoader->setValue("input", "invertYAxis", false, false, false);
    engine::settingsLoader->addCategory("ui");
    engine::settingsLoader->setValue("ui", "language", std::string("en"), false, false);
    engine::settingsLoader->addCategory("scripting");
    engine::settingsLoader->setValue("scripting", "angelscript", true, false, false);
    engine::settingsLoader->save();
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
    image icon(
            ((filesystemResourceProvider*) resourceManager::getResourceProviderWithResource("file", iconPath))->getPath() + "/" + iconPath,
            &width, &height, &bitsPerPixel, 4, false);
#if DEBUG
    assert(icon.getData());
#endif
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = icon.getData();
    glfwSetWindowIcon(this->window, 1, images);
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
