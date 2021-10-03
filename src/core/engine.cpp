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
#include "../resource/internetResourceProvider.h"
#include "../loader/objMeshLoader.h"
#include "../loader/primitiveMeshLoader.h"
#include "../render/texture2d.h"
#include "../render/texturedMaterial.h"
#include "../events/eventQueue.h"

#if __has_include(<windows.h>)
#include <windows.h>
#endif

using namespace chira;

void engine::preInit(const std::string& configPath) {
#ifdef _WIN32
    system(("chcp " + std::to_string(CP_UTF8) + " > nul").c_str());
#ifndef DEBUG
    FreeConsole();
#endif
#endif
    resourceManager::addResourceProvider("file", new filesystemResourceProvider{"file", ENGINE_FILESYSTEM_PATH});
    resourceManager::addResourceProvider("http", new internetResourceProvider{"http", 80});
    resourceManager::addResourceProvider("https", new internetResourceProvider{"https", 443});
    engine::setSettingsLoader(new jsonSettingsLoader(configPath));
    // todo: use computer language as default here
    std::string defaultLang = "en";
    engine::getSettingsLoader()->getValue("ui", "language", &defaultLang);
    translationManager::setLanguage(defaultLang);
    translationManager::addTranslationFile("file://i18n/engine");
    engine::lastTime = 0;
    engine::currentTime = 0;
    engine::lastMouseX = -1;
    engine::lastMouseY = -1;
}

void engine::errorCallback(int error, const char* description) {
    fmt::print(stderr, TR("error.glfw.generic"), error, description);
}

#if DEBUG
void APIENTRY glDebugOutputCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
    // Leaving OpenGL error reports unlocalized is probably best

    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        default:                              std::cout << "Source: Other";
    } std::cout << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        default:                                std::cout << "Type: Other";
    } std::cout << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        default:                             std::cout << "Severity: other";
    } std::cout << std::endl << std::endl;
}
#endif

void engine::framebufferSizeCallback(GLFWwindow* w, int width, int height) {
    glViewport(0, 0, width, height);
}

void engine::keyboardCallback(GLFWwindow* w, int key, int scancode, int action, int mods) {
    if (action == GLFW_REPEAT) return;
    for (keybind& k : *engine::getKeybinds()) {
        if (k.getButton() == key && k.getAction() == action) {
            k.run();
        }
    }
}

void engine::keyboardRepeatingCallback() {
    for (keybind& k : engine::keybinds) {
        if (glfwGetKey(engine::window, k.getButton()) && k.getAction() == GLFW_REPEAT) {
            k.run();
        }
    }
}

void engine::mouseButtonCallback(GLFWwindow* w, int button, int action, int mods) {
    for (keybind& k : *engine::getKeybinds()) {
        if (k.getButton() == button && k.getAction() == action) {
            k.run();
        }
    }
}

void engine::mouseButtonRepeatingCallback() {
    for (keybind& k : engine::keybinds) {
        if (k.isMouse() && (glfwGetMouseButton(engine::window, k.getButton()) && k.getAction() == GLFW_REPEAT)) {
            k.run();
        }
    }
}

void engine::mouseMovementCallback(GLFWwindow* w, double xPos, double yPos) {
    if (engine::lastMouseX == -1) engine::lastMouseX = xPos;
    if (engine::lastMouseY == -1) engine::lastMouseY = yPos;

    int width, height;
    glfwGetWindowSize(engine::window, &width, &height);
    double xOffset = xPos - engine::lastMouseX;
    double yOffset = yPos - engine::lastMouseY;

    for (mousebind& bind : *engine::getMousebinds()) {
        if (bind.getType() == MOVE) {
            bind.run(xOffset, yOffset);
        }
    }

    engine::lastMouseX = xPos;
    engine::lastMouseY = yPos;
}

void engine::mouseScrollCallback(GLFWwindow* w, double xPos, double yPos) {
    for (mousebind& bind : *engine::getMousebinds()) {
        if (bind.getType() == SCROLL) {
            bind.run(xPos, yPos);
        }
    }
}

void engine::windowIconifyCallback(GLFWwindow* w, int isIconified) {
    engine::iconified = (isIconified == GLFW_TRUE);
}

void engine::init() {
    engine::started = true;

    logger::addCallback([=](const loggerType& type, const std::string& source, const std::string& message) {
        engine::getConsole()->engineLoggingHook(type, source, message);
    });

    if (!glfwInit()) {
        logger::log(ERR, "GLFW", TR("error.glfw.undefined"));
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(engine::errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    int windowWidth = 1600;
    engine::getSettingsLoader()->getValue("graphics", "windowWidth", &windowWidth);
    int windowHeight = 900;
    engine::getSettingsLoader()->getValue("graphics", "windowHeight", &windowHeight);
    bool fullscreen = false;
    engine::getSettingsLoader()->getValue("graphics", "fullscreen", &fullscreen);
    engine::window = glfwCreateWindow(windowWidth, windowHeight, TR("ui.window.title").c_str(), nullptr, nullptr);
    if (fullscreen) {
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwSetWindowMonitor(engine::window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    if (!engine::window) {
        logger::log(ERR, "GLFW", TR("error.glfw.window"));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(engine::window);

    bool startMaximized = true;
    engine::getSettingsLoader()->getValue("graphics", "startMaximized", &startMaximized);
    if (startMaximized && !fullscreen) {
        glfwMaximizeWindow(engine::window);
    }

    if (engine::getSettingsLoader()->hasValue("engine", "iconPath")) {
        std::string path{};
        engine::getSettingsLoader()->getValue("engine", "iconPath", &path);
        engine::setIcon(path);
    } else {
        logger::log(WARN, "ChiraEngine", TR("error.engine.unset_icon_path"));
    }

#if DEBUG
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    logger::log(INFO, "GLFW", fmt::format(TR("debug.glfw.version"), major, minor, rev));
#endif

    if (!gladLoadGL(glfwGetProcAddress)) {
        logger::log(ERR, "OpenGL", fmt::format("error.opengl.version", GL_VERSION_STRING_PRETTY));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

#if DEBUG
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutputCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    int vertexAttributes, textureUnits;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &vertexAttributes);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    logger::log(INFO, "OpenGL", fmt::format(TR("debug.opengl.vertex_attributes"), vertexAttributes));
    logger::log(INFO, "OpenGL", fmt::format(TR("debug.opengl.texture_units"), textureUnits));
#endif

    int width, height;
    glfwGetFramebufferSize(engine::window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(engine::window, engine::framebufferSizeCallback);
    engine::setBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

    mesh::addMeshLoader("primitive", new primitiveMeshLoader{});
    mesh::addMeshLoader("obj", new objMeshLoader{});

    engine::displaySplashScreen();
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(engine::window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(engine::window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    bool rawMouseMotion = false;
    engine::getSettingsLoader()->getValue("input", "rawMouseMotion", &rawMouseMotion);
    if (glfwRawMouseMotionSupported() && rawMouseMotion) {
        glfwSetInputMode(engine::window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwSetKeyCallback(engine::window, engine::keyboardCallback);
    glfwSetMouseButtonCallback(engine::window, engine::mouseButtonCallback);
    glfwSetCursorPosCallback(engine::window, engine::mouseMovementCallback);
    glfwSetScrollCallback(engine::window, engine::mouseScrollCallback);
    glfwSetWindowIconifyCallback(engine::window, engine::windowIconifyCallback);

#if DEBUG
    IMGUI_CHECKVERSION();
#endif
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(engine::window, true);
    io.Fonts->Clear();
    ImGui_ImplOpenGL3_Init(GL_VERSION_STRING.data());
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    logger::log(INFO, "ImGUI", TR("debug.imgui.success"));

    bool openalEnabled = true;
    engine::getSettingsLoader()->getValue("scripting", "angelscript", &openalEnabled);
    if (openalEnabled) {
        engine::setSoundManager(new alSoundManager{});
    }
    engine::soundManager->init();

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

    engine::angelscript = std::make_unique<angelscriptProvider>();
    engine::angelscript->initProvider();
    engine::angelscript->registerGlobalFunction(engine::setBackgroundColor, "setBackgroundColor");
    engine::angelscript->registerGlobalFunction(engine::setBackgroundColor, "captureMouse");
    engine::angelscript->registerGlobalFunction(engine::setBackgroundColor, "isMouseCaptured");
    engine::angelscript->registerGlobalFunction(engine::setBackgroundColor, "showConsole");
    // Reminder on how to define a callable method:
    // engine::angelscript->asEngine->RegisterGlobalFunction("void showConsole(bool)", asMETHOD(engine, showConsole), asCALL_THISCALL_ASGLOBAL, this);

    io.Fonts->AddFontDefault();

    engine::callRegisteredFunctions(&(engine::initFunctions));
    engine::angelscript->initScripts();

    resourceManager::precacheResource<fontResource>(TR("resource.font.console_font_path"));
    io.Fonts->Build();
}

void engine::displaySplashScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto* mat = resourceManager::getResource<texturedMaterial>("file://materials/splashscreen.json");
    auto* plane = resourceManager::getResource<mesh>("file://meshes/plane.json", mat);
    plane->render();

    glfwSwapBuffers(engine::window);
    plane->release();
}

void engine::run() {
    engine::lastTime = engine::currentTime;
    engine::currentTime = glfwGetTime();

    while (!glfwWindowShouldClose(engine::window)) {
        engine::render();
        engine::soundManager->setListenerPosition(engine::getMainCamera()->getPosition());
        engine::soundManager->setListenerRotation(engine::getMainCamera()->getRotation(), engine::getMainCamera()->getUpVector());
        engine::soundManager->update();
        glfwSwapBuffers(engine::window);
        glfwPollEvents();
        engine::keyboardRepeatingCallback();
        engine::mouseButtonRepeatingCallback();
        if (discordRichPresence::initialized()) {
            discordRichPresence::updatePresence();
        }
        eventQueue::flushEvents();
    }

    engine::stop();
}

void engine::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    engine::lastTime = engine::currentTime;
    engine::currentTime = glfwGetTime();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    engine::callRegisteredFunctions(&(engine::renderFunctions));
    engine::angelscript->render(engine::getDeltaTime());

    if (engine::getConsole()->getEnabled()) {
        engine::getConsole()->render();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void engine::stop() {
    logger::log(INFO_IMPORTANT, "ChiraEngine", TR("debug.engine.exit"));

    engine::callRegisteredFunctions(&(engine::stopFunctions));
    engine::angelscript->stop();

    if (discordRichPresence::initialized()) {
        discordRichPresence::shutdown();
    }

    engine::soundManager->stop();

    resourceManager::discardAll();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(engine::window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void engine::addInitFunction(const std::function<void()>& init) {
    engine::initFunctions.push_back(init);
}

void engine::addRenderFunction(const std::function<void()>& render) {
    engine::renderFunctions.push_back(render);
}

void engine::addStopFunction(const std::function<void()>& stop) {
    engine::stopFunctions.push_back(stop);
}

void engine::setBackgroundColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void engine::addKeybind(const keybind& keybind) {
    engine::keybinds.push_back(keybind);
}

std::vector<keybind>* engine::getKeybinds() {
    return &(engine::keybinds);
}

void engine::addMousebind(const mousebind& mousebind) {
    engine::mousebinds.push_back(mousebind);
}

std::vector<mousebind>* engine::getMousebinds() {
    return &(engine::mousebinds);
}

angelscriptProvider* engine::getAngelscriptProvider() {
    if (!engine::angelscript) {
        logger::log(ERR, "engine::getAngelscriptProvider", fmt::format(TR("error.engine.script_provider_missing"), "AngelScript"));
    }
    return engine::angelscript.get();
}

void engine::setSoundManager(abstractSoundManager* newSoundManager) {
    engine::soundManager.reset(newSoundManager);
}

abstractSoundManager* engine::getSoundManager() {
    if (!engine::soundManager) {
        logger::log(WARN, "engine::getSoundManager", fmt::format(TR("error.engine.invalid_access"), "sound manager", "engine::setSoundManager"));
        return nullptr;
    }
    return engine::soundManager.get();
}

abstractSettingsLoader* engine::getSettingsLoader() {
    if (!engine::settingsLoader) {
        logger::log(WARN, "engine::getSettingsLoader", fmt::format(TR("error.engine.invalid_access"), "settings loader", "engine::setSettingsLoader"));
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
        logger::log(WARN, "engine::getPhysicsProvider", fmt::format(TR("error.engine.invalid_access"), "physics provider", "engine::setPhysicsProvider"));
        return nullptr;
    }
    return engine::physicsProvider.get();
}

void engine::setPhysicsProvider(abstractPhysicsProvider* newPhysicsProvider) {
    engine::physicsProvider.reset(newPhysicsProvider);
}

abstractCamera* engine::getMainCamera() {
    if (!engine::mainCamera) {
        logger::log(WARN, "engine::getMainCamera", fmt::format(TR("error.engine.invalid_access"), "main camera", "engine::setMainCamera"));
        return nullptr;
    }
    return engine::mainCamera;
}

void engine::setMainCamera(abstractCamera* newCamera) {
    if (engine::mainCamera) {
        engine::mainCamera->setCurrent(false);
    }
    engine::mainCamera = newCamera;
    engine::mainCamera->init();
    engine::mainCamera->setCurrent(true);
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
    engine::settingsLoader->setValue("graphics", "startMaximized", false, false, false);
    engine::settingsLoader->setValue("graphics", "fullscreen", false, false, false);
    engine::settingsLoader->addCategory("input");
    engine::settingsLoader->setValue("input", "rawMouseMotion", true, false, false);
    engine::settingsLoader->setValue("input", "invertYAxis", false, false, false);
    engine::settingsLoader->addCategory("ui");
    engine::settingsLoader->setValue("ui", "language", std::string("en"), false, false);
    engine::settingsLoader->save();
}

void engine::callRegisteredFunctions(const std::vector<std::function<void()>>* list) {
    for (const auto& func : *list) {
        func();
    }
}

const GLFWwindow* engine::getWindow() {
    return engine::window;
}

bool engine::isStarted() {
    return engine::started;
}

double engine::getDeltaTime() {
    return engine::currentTime - engine::lastTime;
}

void engine::setIcon(const std::string& iconPath) {
#if DEBUG
    assert(engine::started);
#endif
    GLFWimage images[1];
    int width, height, bitsPerPixel;
    image icon(
            ((filesystemResourceProvider*) resourceManager::getResourceProviderWithResource("file://" + iconPath))->getPath() + "/" + iconPath,
            &width, &height, &bitsPerPixel, 4, false);
#if DEBUG
    assert(icon.getData());
#endif
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = icon.getData();
    glfwSetWindowIcon(engine::window, 1, images);
}

void engine::captureMouse(bool capture) {
    engine::mouseCaptured = capture;
    if (capture) {
        glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    } else {
        glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
}

bool engine::isMouseCaptured() {
    return engine::mouseCaptured;
}

void engine::showConsole(bool shouldShow) {
    engine::consoleUI.setEnabled(shouldShow);
}

console* engine::getConsole() {
    return &(engine::consoleUI);
}

bool engine::isIconified() {
    return engine::iconified;
}
