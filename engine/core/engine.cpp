#include "engine.h"

#include <glad/gl.h>
#define IMGUI_USER_CONFIG <config/imguiConfig.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <i18n/translationManager.h>
#include <config/glVersion.h>
#include <input/inputManager.h>
#include <loader/settings/jsonSettingsLoader.h>
#include <loader/image/image.h>
#include <sound/alSoundManager.h>
#include <hook/discordRPC.h>
#include <resource/provider/filesystemResourceProvider.h>
#include <resource/provider/internetResourceProvider.h>
#include <resource/shaderResource.h>
#include <loader/mesh/objMeshLoader.h>
#include <loader/mesh/chiraMeshLoader.h>
#include <loader/mesh/primitiveMeshLoader.h>
#include <physics/bulletPhysicsProvider.h>
#include <render/ubo.h>
#include <event/events.h>
#include <entity/imgui/console/console.h>
#include <entity/imgui/profiler/profiler.h>
#include <utility/debug/assertions.h>
#if __has_include(<windows.h>) && !defined(DEBUG)
#include <windows.h>
#undef ERROR
#endif
#ifdef CHIRA_BUILD_WITH_STEAMWORKS
#include <hook/steamAPI.h>
#endif

using namespace chira;

void Engine::preInit(const std::string& configPath) {
#ifdef _WIN32
    // #define CP_UTF8 65001 in windows.h
    system(("chcp " + std::to_string(65001) + " > nul").c_str());
#ifndef DEBUG
    FreeConsole();
#endif
#endif
    Resource::addResourceProvider(new FilesystemResourceProvider{ENGINE_FILESYSTEM_PATH});
    Resource::addResourceProvider(new InternetResourceProvider{"http", 80});
    Resource::addResourceProvider(new InternetResourceProvider{"https", 443});
    Engine::setSettingsLoader(new JSONSettingsLoader{configPath});
    std::string defaultLang;
    Engine::getSettingsLoader()->getValue("ui", "language", &defaultLang);
    TranslationManager::setLanguage(defaultLang);
    TranslationManager::addTranslationFile("file://i18n/engine");
    Engine::lastTime = 0;
    Engine::currentTime = 0;
    Engine::lastMouseX = -1;
    Engine::lastMouseY = -1;
}

void Engine::displaySplashScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto mat = Resource::getResource<MaterialTextured>(TR("resource.material.splashscreen_material_path"));
    auto plane = Resource::getResource<MeshResource>("file://meshes/plane.json");
    plane->setMaterial(mat.castAssert<MaterialBase>());
    plane->render(glm::identity<glm::mat4>());
    glfwSwapBuffers(Engine::window);
}

void Engine::init() {
    Engine::started = true;

    Engine::console = new Console{};
#ifdef DEBUG
    Engine::profiler = new Profiler{};
#endif

    if (!glfwInit()) {
        Logger::log(LogType::ERROR, "GLFW", TR("error.glfw.undefined"));
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback([](int error, const char* description) {
        Logger::log(LogType::ERROR, "GLFW", TRF("error.glfw.generic", error, description));
    });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    int windowWidth = 1600;
    Engine::getSettingsLoader()->getValue("graphics", "windowWidth", &windowWidth);
    int windowHeight = 900;
    Engine::getSettingsLoader()->getValue("graphics", "windowHeight", &windowHeight);
    bool fullscreen = false;
    Engine::getSettingsLoader()->getValue("graphics", "fullscreen", &fullscreen);
    Engine::window = glfwCreateWindow(windowWidth, windowHeight, TRC("ui.window.title"), nullptr, nullptr);
    if (!Engine::window) {
        Logger::log(LogType::ERROR, "GLFW", TR("error.glfw.window"));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    if (fullscreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwSetWindowMonitor(Engine::window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        bool startMaximized = true;
        Engine::getSettingsLoader()->getValue("graphics", "startMaximized", &startMaximized);
        if (startMaximized)
            glfwMaximizeWindow(Engine::window);
    }
    glfwMakeContextCurrent(Engine::window);
    Engine::setIcon("file://textures/ui/icon.png");

    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    Logger::log(LogType::INFO, "GLFW", TRF("debug.glfw.version", major, minor, rev));

    if (!gladLoadGL(glfwGetProcAddress)) {
        Logger::log(LogType::ERROR, "OpenGL", fmt::format("error.opengl.version", GL_VERSION_STRING_PRETTY));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback([](GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
            // Leaving OpenGL error reports unlocalized is probably best

            if (id == 8 || id == 131169 || id == 131185 || id == 131218 || id == 131204) {
                // Ignore 8 because the Steam overlay tries to bind to an already bound framebuffer, very low overhead, don't worry about it
                // Others are ignored because learnopengl.com said they were duplicates
                return;
            }
            std::string output = "---------------\nDebug message (" + std::to_string(id) + "): " +  message;

            output += "\nSource: ";
            switch (source) {
                case GL_DEBUG_SOURCE_API:             output += "API"; break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   output += "Window System"; break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER: output += "Shader Compiler"; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:     output += "Third Party"; break;
                case GL_DEBUG_SOURCE_APPLICATION:     output += "Application"; break;
                default:                              output += "Other";
            }
            output += "\nType: ";
            switch (type) {
                case GL_DEBUG_TYPE_ERROR:               output += "Error"; break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: output += "Deprecated Behaviour"; break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  output += "Undefined Behaviour"; break;
                case GL_DEBUG_TYPE_PORTABILITY:         output += "Portability"; break;
                case GL_DEBUG_TYPE_PERFORMANCE:         output += "Performance"; break;
                case GL_DEBUG_TYPE_MARKER:              output += "Marker"; break;
                case GL_DEBUG_TYPE_PUSH_GROUP:          output += "Push Group"; break;
                case GL_DEBUG_TYPE_POP_GROUP:           output += "Pop Group"; break;
                default:                                output += "Other";
            }
            output += "\nSeverity: ";
            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:         output += "High"; break;
                case GL_DEBUG_SEVERITY_MEDIUM:       output += "Medium"; break;
                case GL_DEBUG_SEVERITY_LOW:          output += "Low"; break;
                case GL_DEBUG_SEVERITY_NOTIFICATION: output += "Notification"; break;
                default:                             output += "Other";
            }

            if (type == GL_DEBUG_TYPE_ERROR)
                Logger::log(LogType::ERROR, "OpenGL", output);
            else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
                Logger::log(LogType::INFO, "OpenGL", output);
            else
                // Logging as a warning because most of the time the program runs perfectly fine
                Logger::log(LogType::WARNING, "OpenGL", output);
        }, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    int width, height;
    glfwGetFramebufferSize(Engine::window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(Engine::window, [](GLFWwindow* w, int width, int height) {
        glViewport(0, 0, width, height);
        if (Engine::root && Engine::root->getMainCamera())
            Engine::root->getMainCamera()->createProjection(width, height);
    });
    Engine::setBackgroundColor(ColorRGB{});

    MeshResource::addMeshLoader("primitive", new PrimitiveMeshLoader{});
    MeshResource::addMeshLoader("obj", new OBJMeshLoader{});
    MeshResource::addMeshLoader("cmdl", new ChiraMeshLoader{});

    Engine::displaySplashScreen();
    Resource::cleanup();

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // Wiki says modern hardware is fine with this and it looks better

    glfwSetInputMode(Engine::window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(Engine::window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    bool rawMouseMotion = false;
    Engine::getSettingsLoader()->getValue("input", "rawMouseMotion", &rawMouseMotion);
    if (glfwRawMouseMotionSupported() && rawMouseMotion)
        glfwSetInputMode(Engine::window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetKeyCallback(Engine::window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        if (action == GLFW_REPEAT) return;
        for (auto& keybind : InputManager::getKeyButtonCallbacks()) {
            if (keybind.getKey() == static_cast<Key>(key) && keybind.getEventType() == static_cast<InputKeyEventType>(action))
                keybind();
        }
    });
    glfwSetMouseButtonCallback(Engine::window, [](GLFWwindow* w, int key, int action, int mods) {
        if (action == GLFW_REPEAT) return;
        for (auto& keybind : InputManager::getMouseButtonCallbacks()) {
            if (keybind.getKey() == static_cast<Key>(key) && keybind.getEventType() == static_cast<InputKeyEventType>(action))
                keybind();
        }
    });
    glfwSetCursorPosCallback(Engine::window, [](GLFWwindow* w, double xPos, double yPos) {
        if (Engine::lastMouseX == -1) Engine::lastMouseX = xPos;
        if (Engine::lastMouseY == -1) Engine::lastMouseY = yPos;

        int width, height;
        glfwGetWindowSize(Engine::window, &width, &height);
        double xOffset = xPos - Engine::lastMouseX;
        double yOffset = yPos - Engine::lastMouseY;

        for (auto& movebind : InputManager::getMouseMovementCallbacks()) {
            if (movebind.getType() == InputMouseMovementEventType::MOVE)
                movebind(xOffset, yOffset);
        }

        Engine::lastMouseX = xPos;
        Engine::lastMouseY = yPos;
    });
    glfwSetScrollCallback(Engine::window, [](GLFWwindow* w, double x, double y) {
        for (auto& movebind : InputManager::getMouseMovementCallbacks()) {
            if (movebind.getType() == InputMouseMovementEventType::SCROLL)
                movebind(x,y);
        }
    });
    glfwSetWindowIconifyCallback(Engine::window, [](GLFWwindow* w, int isIconified) {
        Engine::iconified = (isIconified == GLFW_TRUE);
    });

#ifdef DEBUG
    IMGUI_CHECKVERSION();
#endif
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(Engine::window, true);
    io.Fonts->Clear();
    ImGui_ImplOpenGL3_Init(GL_VERSION_STRING.data());
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    bool openalEnabled = true;
    Engine::getSettingsLoader()->getValue("audio", "openal", &openalEnabled);
    if (openalEnabled)
        Engine::setSoundManager(new ALSoundManager{});
    Engine::soundManager->init();

    // todo: move this to a general lighting manager
    if (Engine::getSettingsLoader()->hasValue("engine", "maxPointLights")) {
        int maxLights;
        Engine::getSettingsLoader()->getValue("engine", "maxPointLights", &maxLights);
        ShaderResource::addPreprocessorSymbol("MAX_POINT_LIGHTS", std::to_string(maxLights));
    }
    if (Engine::getSettingsLoader()->hasValue("engine", "maxPointLights")) {
        int maxLights;
        Engine::getSettingsLoader()->getValue("engine", "maxDirectionalLights", &maxLights);
        ShaderResource::addPreprocessorSymbol("MAX_DIRECTIONAL_LIGHTS", std::to_string(maxLights));
    }
    if (Engine::getSettingsLoader()->hasValue("engine", "maxSpotLights")) {
        int maxLights;
        Engine::getSettingsLoader()->getValue("engine", "maxSpotLights", &maxLights);
        ShaderResource::addPreprocessorSymbol("MAX_SPOT_LIGHTS", std::to_string(maxLights));
    }

    bool bulletEnabled = true;
    Engine::getSettingsLoader()->getValue("physics", "bullet", &bulletEnabled);
    if (bulletEnabled)
        Engine::setPhysicsProvider(new BulletPhysicsProvider{});

#ifdef CHIRA_BUILD_WITH_STEAMWORKS
    bool steamEnabled = false;
    Engine::getSettingsLoader()->getValue("engine", "steamworks", &steamEnabled);
    if (steamEnabled && !SteamAPI::Client::initSteam())
        Logger::log(LogType::ERROR, "Steam", TR("error.steam.initialization_failure"));
#endif

    Engine::angelscript = std::make_unique<AngelscriptProvider>();
    Engine::angelscript->initProvider();
    // Static function:
    //Engine::angelscript->registerGlobalFunction(Engine::setBackgroundColor, "setBackgroundColor");
    // Method:
    //Engine::angelscript->asEngine->RegisterGlobalFunction("void showConsole(bool)", asMETHOD(Engine, showConsole), asCALL_THISCALL_ASGLOBAL, this);

    io.Fonts->AddFontDefault();
    Engine::console->precacheResource();
    auto defaultFont = Resource::getResource<FontResource>("file://fonts/default.json");
    ImGui::GetIO().FontDefault = defaultFont->getFont();

    Engine::root = new Root{"root"};
    Engine::root->addChild(Engine::console);
#if DEBUG
    Engine::root->addChild(Engine::profiler);
#endif
    Engine::callRegisteredFunctions(Engine::initFunctions);
    Engine::angelscript->initScripts();

    io.Fonts->Build();
}

void Engine::run() {
    Engine::lastTime = Engine::currentTime;
    Engine::currentTime = glfwGetTime();

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Engine::lastTime = Engine::currentTime;
        Engine::currentTime = glfwGetTime();

        Engine::physicsProvider->updatePhysics(Engine::getDeltaTime());

        UBO_PV::get()->update(Engine::getRoot()->getMainCamera()->getProjection(), Engine::getRoot()->getMainCamera()->getView());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Engine::callRegisteredFunctions(Engine::renderFunctions);
        Engine::angelscript->render(Engine::getDeltaTime());
        Engine::root->render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        Engine::soundManager->setListenerPosition(Engine::getRoot()->getAudioListeningPosition());
        Engine::soundManager->setListenerRotation(Engine::getRoot()->getAudioListeningRotation(), Engine::getRoot()->getAudioListeningUpVector());
        Engine::soundManager->update();

        glfwPollEvents();
        for (auto& keybind : InputManager::getKeyButtonCallbacks()) {
            if (glfwGetKey(Engine::window, static_cast<int>(keybind.getKey())) && keybind.getEventType() == InputKeyEventType::REPEAT)
                keybind();
        }
        for (auto& keybind : InputManager::getMouseButtonCallbacks()) {
            if (glfwGetMouseButton(Engine::window, static_cast<int>(keybind.getKey())) && keybind.getEventType() == InputKeyEventType::REPEAT)
                keybind();
        }

        if (DiscordRPC::initialized())
            DiscordRPC::updatePresence();
#ifdef CHIRA_BUILD_WITH_STEAMWORKS
        if (SteamAPI::Client::initialized())
            SteamAPI::Client::runCallbacks();
#endif
        Events::update();
        Resource::cleanup();

        glfwSwapBuffers(Engine::window);
    } while (!glfwWindowShouldClose(Engine::window));
    Engine::stop();
}

void Engine::stop() {
    Logger::log(LogType::INFO_IMPORTANT, "Engine", TR("debug.engine.exit"));

    Engine::callRegisteredFunctions(Engine::stopFunctions);
    Engine::angelscript->stop();

    if (DiscordRPC::initialized())
        DiscordRPC::shutdown();
#ifdef CHIRA_BUILD_WITH_STEAMWORKS
    if (SteamAPI::Client::initialized())
        SteamAPI::Client::shutdown();
#endif

    Engine::soundManager->stop();
    delete Engine::root;
    Engine::physicsProvider->stop();
    Resource::discardAll();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(Engine::window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Engine::addInitFunction(const std::function<void()>& init) {
    Engine::initFunctions.push_back(init);
}

void Engine::addRenderFunction(const std::function<void()>& render) {
    Engine::renderFunctions.push_back(render);
}

void Engine::addStopFunction(const std::function<void()>& stop) {
    Engine::stopFunctions.push_back(stop);
}

void Engine::setBackgroundColor(ColorRGB color) {
    glClearColor(color.r, color.g, color.b, 1.f);
}

glm::vec2 Engine::getWindowSize() {
    int w, h;
    glfwGetWindowSize(Engine::window, &w, &h);
    return {w, h};
}

int Engine::getWindowWidth() {
    int w, h;
    glfwGetWindowSize(Engine::window, &w, &h);
    return w;
}

int Engine::getWindowHeight() {
    int w, h;
    glfwGetWindowSize(Engine::window, &w, &h);
    return h;
}

void Engine::setWindowSize(int width, int height) {
    glfwSetWindowSize(Engine::window, width, height);
}

void Engine::shouldStopAfterThisFrame(bool yes) {
    glfwSetWindowShouldClose(Engine::window, yes ? GLFW_TRUE : GLFW_FALSE);
}

AngelscriptProvider* Engine::getAngelscriptProvider() {
    if (!Engine::angelscript) {
        Logger::log(LogType::ERROR, "Engine::getAngelscriptProvider", TRF("error.engine.script_provider_missing", "AngelScript"));
    }
    return Engine::angelscript.get();
}

AbstractSoundManager* Engine::getSoundManager() {
    if (!Engine::soundManager) {
        Logger::log(LogType::WARNING, "Engine::getSoundManager", TRF("error.engine.invalid_access", "sound manager", "Engine::setSoundManager"));
        return nullptr;
    }
    return Engine::soundManager.get();
}

void Engine::setSoundManager(AbstractSoundManager* newSoundManager) {
    Engine::soundManager.reset(newSoundManager);
}

AbstractSettingsLoader* Engine::getSettingsLoader() {
    if (!Engine::settingsLoader) {
        Logger::log(LogType::WARNING, "Engine::getSettingsLoader", TRF("error.engine.invalid_access", "settings loader", "Engine::setSettingsLoader"));
        return nullptr;
    }
    return Engine::settingsLoader.get();
}

void Engine::setSettingsLoader(AbstractSettingsLoader* newSettingsLoader) {
    Engine::settingsLoader.reset(newSettingsLoader);
    Engine::setSettingsLoaderDefaults();
}

AbstractPhysicsProvider* Engine::getPhysicsProvider() {
    if (!Engine::physicsProvider) {
        Logger::log(LogType::WARNING, "Engine::getPhysicsProvider", TRF("error.engine.invalid_access", "physics provider", "Engine::setPhysicsProvider"));
        return nullptr;
    }
    return Engine::physicsProvider.get();
}

void Engine::setPhysicsProvider(AbstractPhysicsProvider* newPhysicsProvider) {
    Engine::physicsProvider.reset(newPhysicsProvider);
}

void Engine::setSettingsLoaderDefaults() {
    Engine::settingsLoader->load();
    Engine::settingsLoader->addCategory("engine");
    Engine::settingsLoader->setValue("engine", "consoleColoredText", true, false, false);
    Engine::settingsLoader->setValue("engine", "maxPointLights", 64, false, false);
    Engine::settingsLoader->setValue("engine", "maxDirectionalLights", 4, false, false);
    Engine::settingsLoader->setValue("engine", "maxSpotLights", 4, false, false);
#ifdef CHIRA_BUILD_WITH_STEAMWORKS
    Engine::settingsLoader->setValue("engine", "steamworks", false, false, false);
#endif
    Engine::settingsLoader->addCategory("audio");
    Engine::settingsLoader->setValue("audio", "openal", true, false, false);
    Engine::settingsLoader->addCategory("physics");
    Engine::settingsLoader->setValue("physics", "bullet", true, false, false);
    Engine::getSettingsLoader()->setValue("physics", "subStep", 4, false, false);
    Engine::settingsLoader->addCategory("graphics");
    Engine::settingsLoader->setValue("graphics", "windowWidth", 1600, false, false);
    Engine::settingsLoader->setValue("graphics", "windowHeight", 900, false, false);
    Engine::settingsLoader->setValue("graphics", "startMaximized", false, false, false);
    Engine::settingsLoader->setValue("graphics", "fullscreen", false, false, false);
    Engine::settingsLoader->addCategory("input");
    Engine::settingsLoader->setValue("input", "rawMouseMotion", true, false, false);
    Engine::settingsLoader->setValue("input", "invertYAxis", false, false, false);
    Engine::settingsLoader->addCategory("ui");
    // todo: use computer language as default
    Engine::settingsLoader->setValue("ui", "language", std::string{"en"}, false, false);
    Engine::settingsLoader->save();
}

void Engine::callRegisteredFunctions(const std::vector<std::function<void()>>& list) {
    for (const auto& func : list) {
        func();
    }
}

Root* Engine::getRoot() {
    return Engine::root;
}

Console* Engine::getConsole() {
    return Engine::console;
}

Profiler* Engine::getProfiler() {
#if DEBUG
    return Engine::profiler;
#else
    Logger::log(LogType::ERROR, "Engine::getProfiler", "Profiler window is not present in release build!");
    return nullptr;
#endif
}

bool Engine::isStarted() {
    return Engine::started;
}

double Engine::getDeltaTime() {
    return Engine::currentTime - Engine::lastTime;
}

void Engine::captureMouse(bool capture) {
    Engine::mouseCaptured = capture;
    if (capture) {
        glfwSetInputMode(Engine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    } else {
        glfwSetInputMode(Engine::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
}

bool Engine::isMouseCaptured() {
    return Engine::mouseCaptured;
}

bool Engine::isIconified() {
    return Engine::iconified;
}

void Engine::setIcon(const std::string& iconPath) {
    chira_assert(Engine::isStarted(), TR("error.engine.not_initialized"));
    GLFWimage images[1];
    int width, height, bitsPerPixel;
    Image icon(FilesystemResourceProvider::getResourceAbsolutePath(iconPath), &width, &height, &bitsPerPixel, 4, false);
    chira_assert(icon.getData(), TR("error.engine.icon_has_no_data"));
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = icon.getData();
    glfwSetWindowIcon(Engine::window, 1, images);
}
