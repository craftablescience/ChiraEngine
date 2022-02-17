#include "engine.h"

#include <glad/gl.h>

#include <config/glVersion.h>
#include <event/events.h>
#include <hook/discordRPC.h>
#ifdef CHIRA_BUILD_WITH_STEAMWORKS
#include <hook/steamAPI.h>
#endif
#include <i18n/translationManager.h>
#include <input/inputManager.h>
#include <loader/mesh/objMeshLoader.h>
#include <loader/mesh/chiraMeshLoader.h>
#include <loader/settings/jsonSettingsLoader.h>
#include <physics/bulletPhysicsProvider.h>
#include <resource/provider/filesystemResourceProvider.h>
#ifdef CHIRA_BUILD_WITH_NETWORKING
#include <resource/provider/internetResourceProvider.h>
#endif
#include <resource/shaderResource.h>
#include <render/ubo.h>
#include <sound/alSoundManager.h>
#include <utility/debug/assertions.h>

#if __has_include(<windows.h>) && !defined(DEBUG)
#include <windows.h>
#undef ERROR
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
#ifdef CHIRA_BUILD_WITH_NETWORKING
    Resource::addResourceProvider(new InternetResourceProvider{"http", 80});
    Resource::addResourceProvider(new InternetResourceProvider{"https", 443});
#endif
    Engine::setSettingsLoader(new JSONSettingsLoader{configPath});
    std::string defaultLang;
    Engine::getSettingsLoader()->getValue("ui", "language", &defaultLang);
    TranslationManager::setLanguage(defaultLang);
    TranslationManager::addTranslationFile("file://i18n/engine");
}

void Engine::init(const std::function<void()>& callbackOnInit, bool windowStartsVisible) {
    Engine::started = true;

    if (!glfwInit()) {
        Logger::log(LogType::ERROR, "GLFW", TR("error.glfw.undefined"));
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback([](int error, const char* description) {
        Logger::log(LogType::ERROR, "GLFW", TRF("error.glfw.generic", error, description));
    });

    int windowWidth = 1600;
    Engine::getSettingsLoader()->getValue("graphics", "windowWidth", &windowWidth);
    int windowHeight = 900;
    Engine::getSettingsLoader()->getValue("graphics", "windowHeight", &windowHeight);
    bool fullscreen = false;
    Engine::getSettingsLoader()->getValue("graphics", "fullscreen", &fullscreen);

    Window::getFontAtlasInstance()->AddFontDefault();
#ifdef CHIRA_BUILD_WITH_MULTIWINDOW
    Engine::addWindow(TR("ui.window.title"), windowWidth, windowHeight, fullscreen, {}, true, false);
#else
    Engine::windows.emplace_back(new Window{TR("ui.window.title"), windowWidth, windowHeight, fullscreen, {}, true, false});
#endif
    Engine::getWindow()->setVisible(windowStartsVisible);

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

    IMGUI_CHECKVERSION();
#endif

    Engine::windows[0]->displaySplashScreen();

    AbstractMeshLoader::addMeshLoader("obj", new OBJMeshLoader{});
    AbstractMeshLoader::addMeshLoader("cmdl", new ChiraMeshLoader{});

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

    AbstractPhysicsProvider::setPhysicsProvider(new BulletPhysicsProvider{});

#ifdef CHIRA_BUILD_WITH_STEAMWORKS
    bool steamEnabled = false;
    Engine::getSettingsLoader()->getValue("engine", "steamworks", &steamEnabled);
    if (steamEnabled && (!SteamAPI::Client::initialized() && !SteamAPI::Client::initSteam()))
        Logger::log(LogType::ERROR, "Steam", TR("error.steam.initialization_failure"));
#endif

#ifdef CHIRA_BUILD_WITH_ANGELSCRIPT
    Engine::angelscript = std::make_unique<AngelscriptProvider>();
    // Static function:
    //Engine::angelscript->registerGlobalFunction(Engine::setBackgroundColor, "setBackgroundColor");
    // Method:
    //Engine::angelscript->asEngine->RegisterGlobalFunction("void showConsole(bool)", asMETHOD(Engine, showConsole), asCALL_THISCALL_ASGLOBAL, this);
#endif
    callbackOnInit();
#ifdef CHIRA_BUILD_WITH_ANGELSCRIPT
    Engine::angelscript->init();
#endif

    Window::getFontAtlasInstance()->Build();
}

void Engine::run(const std::function<void()>& callbackOnStop) {
    do {
        Engine::lastTime = Engine::currentTime;
        Engine::currentTime = glfwGetTime();

        AbstractPhysicsProvider::getPhysicsProvider()->updatePhysics(Engine::getDeltaTime());

#ifdef CHIRA_BUILD_WITH_ANGELSCRIPT
        Engine::angelscript->render();
#endif
        for (auto& window : Engine::windows)
            window->render(glm::identity<glm::mat4>());

        glfwPollEvents();
        for (auto& window : Engine::windows) {
            for (auto &keybind: InputManager::getKeyButtonCallbacks()) {
                if (glfwGetKey(window->window, static_cast<int>(keybind.getKey())) && keybind.getEventType() == InputKeyEventType::REPEAT)
                    keybind();
            }
            for (auto &keybind: InputManager::getMouseButtonCallbacks()) {
                if (glfwGetMouseButton(window->window, static_cast<int>(keybind.getKey())) && keybind.getEventType() == InputKeyEventType::REPEAT)
                    keybind();
            }
        }

        //todo(multiview): figure out sound
        Engine::soundManager->setListenerPosition(Engine::getWindow()->getAudioListeningPosition());
        Engine::soundManager->setListenerRotation(Engine::getWindow()->getAudioListeningRotation(), Engine::getWindow()->getAudioListeningUpVector());
        Engine::soundManager->update();

        if (DiscordRPC::initialized())
            DiscordRPC::updatePresence();
#ifdef CHIRA_BUILD_WITH_STEAMWORKS
        if (SteamAPI::Client::initialized())
            SteamAPI::Client::runCallbacks();
#endif
        Events::update();

        if (!Engine::windows.empty()) {
#ifdef CHIRA_BUILD_WITH_MULTIWINDOW
            auto windowIterator = Engine::windows.begin();
            while (windowIterator != Engine::windows.end()) {
                if (glfwWindowShouldClose(windowIterator->get()->window))
                    Engine::windows.erase(windowIterator);
                else
                    windowIterator++;
            }
#else
            if (glfwWindowShouldClose(Engine::windows[0]->window))
                Engine::windows.clear();
#endif
        }
    } while (!Engine::windows.empty());

    Logger::log(LogType::INFO_IMPORTANT, "Engine", TR("debug.engine.exit"));

    callbackOnStop();
#ifdef CHIRA_BUILD_WITH_ANGELSCRIPT
    Engine::angelscript->stop();
#endif

    if (DiscordRPC::initialized())
        DiscordRPC::shutdown();
#ifdef CHIRA_BUILD_WITH_STEAMWORKS
    if (SteamAPI::Client::initialized())
        SteamAPI::Client::shutdown();
#endif

    Engine::soundManager->stop();
    Engine::windows.clear();
    AbstractPhysicsProvider::getPhysicsProvider()->stop();
    Resource::discardAll();

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

#ifdef CHIRA_BUILD_WITH_ANGELSCRIPT
AngelscriptProvider* Engine::getAngelscriptProvider() {
    if (!Engine::angelscript)
        Logger::log(LogType::ERROR, "Engine::getAngelscriptProvider", TRF("error.engine.script_provider_missing", "AngelScript"));
    return Engine::angelscript.get();
}
#endif

AbstractSoundManager* Engine::getSoundManager() {
    if (!Engine::soundManager)
        Logger::log(LogType::WARNING, "Engine::getSoundManager", TRF("error.engine.invalid_access", "sound manager", "Engine::setSoundManager"));
    return Engine::soundManager.get();
}

void Engine::setSoundManager(AbstractSoundManager* newSoundManager) {
    Engine::soundManager.reset(newSoundManager);
}

AbstractSettingsLoader* Engine::getSettingsLoader() {
    if (!Engine::settingsLoader)
        Logger::log(LogType::WARNING, "Engine::getSettingsLoader", TRF("error.engine.invalid_access", "settings loader", "Engine::setSettingsLoader"));
    return Engine::settingsLoader.get();
}

void Engine::setSettingsLoader(AbstractSettingsLoader* newSettingsLoader) {
    Engine::settingsLoader.reset(newSettingsLoader);
    Engine::setSettingsLoaderDefaults();
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
    Engine::settingsLoader->addCategory("physics");
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

Window* Engine::getWindow() {
    if (Engine::windows.empty())
        return nullptr;
    return Engine::windows[0].get();
}

#ifdef CHIRA_BUILD_WITH_MULTIWINDOW
Window* Engine::getWindow(const std::string& name) {
    for (auto& window : Engine::windows) {
        if (window->getName() == name) {
            return window.get();
        }
    }
    Logger::log(LogType::ERROR, "Engine", TRF("error.engine.cannot_find_window", name));
    return Engine::windows[0].get();
}

std::string Engine::addWindow(const std::string& title, int width, int height, bool fullscreen, ColorRGB backgroundColor, bool smoothResize, bool startVisible) {
    Engine::windows.emplace_back(new Window{title, width, height, fullscreen, backgroundColor, smoothResize, startVisible});
    return Engine::windows[Engine::windows.size() - 1]->getName();
}

void Engine::removeWindow(const std::string& name) {
    for (std::size_t i = 0; i < Engine::windows.size(); i++) {
        if (Engine::windows[i]->getName() == name) {
            Engine::windows.erase(Engine::windows.begin() + static_cast<long long int>(i));
            return;
        }
    }
}
#endif

bool Engine::isStarted() {
    return Engine::started;
}

double Engine::getDeltaTime() {
    return Engine::currentTime - Engine::lastTime;
}
