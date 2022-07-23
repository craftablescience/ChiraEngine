#include "Engine.h"

#include <glad/gl.h>
#include <backends/imgui_impl_opengl3.h>

#include <config/Config.h>
#include <hook/DiscordRPC.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include <loader/mesh/OBJMeshLoader.h>
#include <loader/mesh/ChiraMeshLoader.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <resource/ShaderResource.h>
#include <script/AngelScriptVM.h>
#include <ui/debug/ConsolePanel.h>
#include <ui/debug/ResourceUsageTrackerPanel.h>
#include <utility/ConEntry.h>

#ifdef CHIRA_USE_STEAMWORKS
    #include <hook/SteamAPI.h>
#endif

using namespace chira;

[[maybe_unused]]
static ConCommand quit{"quit", "Quits the game or application.", [] { // NOLINT(cert-err58-cpp)
    Engine::getWindow()->shouldStopAfterThisFrame(true);
}};

[[maybe_unused]]
static ConCommand crash{"crash", "Force-crashes the game or application (for debugging purposes).", [] { // NOLINT(cert-err58-cpp)
    throw std::runtime_error{"Called crash command!"};
}, CON_FLAG_CHEAT};

void Engine::preInit(std::string_view configPath) {
#ifdef _WIN32
    // Enable colored text in Windows console by setting encoding to UTF-8
    // #define CP_UTF8 65001 in windows.h
    system("chcp 65001 > nul");
#endif
    Resource::addResourceProvider(new FilesystemResourceProvider{ENGINE_FILESYSTEM_PATH});

    Engine::settingsLoader = std::make_unique<JSONSettingsLoader>(configPath);
    Engine::settingsLoader->load();
    Engine::setSettingsDefaults();

    std::string defaultLang;
    Engine::settingsLoader->getValue("language", &defaultLang);
    TranslationManager::setLanguage(defaultLang);
    TranslationManager::addTranslationFile("file://i18n/engine");
}

void Engine::init(bool windowStartsVisible) {
    Engine::started = true;

    if (!glfwInit()) {
        Logger::log(LOG_ERROR, "GLFW", TR("error.glfw.undefined"));
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback([](int error, const char* description) {
        Logger::log(LOG_ERROR, "GLFW", TRF("error.glfw.generic", error, description));
    });

    int windowWidth = 1600;
    Engine::settingsLoader->getValue("window_width", &windowWidth);
    int windowHeight = 900;
    Engine::settingsLoader->getValue("window_height", &windowHeight);
    bool fullscreen = false;
    Engine::settingsLoader->getValue("fullscreen", &fullscreen);
    bool vsync = true;
    Engine::settingsLoader->getValue("vsync", &vsync);

    Engine::window.reset(new Window{TR("ui.window.title"), windowWidth, windowHeight, fullscreen, vsync});
    Engine::window->setVisible(windowStartsVisible);

#ifdef DEBUG
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback([](GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei /*length*/, const char* message, const void* /*userParam*/) {
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
                Logger::log(LOG_ERROR, "OpenGL", output);
            else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
                Logger::log(LOG_INFO, "OpenGL", output);
            else
                // Logging as a warning because most of the time the program runs perfectly fine
                Logger::log(LOG_WARNING, "OpenGL", output);
        }, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    IMGUI_CHECKVERSION();
#endif

    Engine::window->displaySplashScreen();

    IMeshLoader::addMeshLoader("obj", new OBJMeshLoader{});
    IMeshLoader::addMeshLoader("cmdl", new ChiraMeshLoader{});

    // todo: move this to a general lighting manager
    if (Engine::getSettings()->hasValue("max_point_lights")) {
        int maxLights;
        Engine::getSettings()->getValue("max_point_lights", &maxLights);
        ShaderResource::addPreprocessorSymbol("MAX_POINT_LIGHTS", std::to_string(maxLights));
    }
    if (Engine::getSettings()->hasValue("max_directional_lights")) {
        int maxLights;
        Engine::getSettings()->getValue("max_directional_lights", &maxLights);
        ShaderResource::addPreprocessorSymbol("MAX_DIRECTIONAL_LIGHTS", std::to_string(maxLights));
    }
    if (Engine::getSettings()->hasValue("max_spot_lights")) {
        int maxLights;
        Engine::getSettings()->getValue("max_spot_lights", &maxLights);
        ShaderResource::addPreprocessorSymbol("MAX_SPOT_LIGHTS", std::to_string(maxLights));
    }

#ifdef CHIRA_USE_STEAMWORKS
    bool steamEnabled = false;
    Engine::getSettings()->getValue("steamworks", &steamEnabled);
    if (steamEnabled && (!SteamAPI::Client::initialized() && !SteamAPI::Client::initSteam()))
        Logger::log(LOG_ERROR, "Steam", TR("error.steam.initialization_failure"));
#endif

    // Add console UI panel
    auto consoleID = Engine::getWindow()->addPanel(new ConsolePanel{});
    InputManager::addCallback(InputKeyButton{Key::GRAVE_ACCENT, InputKeyEventType::PRESSED, [consoleID] {
        auto console = Engine::getWindow()->getPanel(consoleID);
        console->setVisible(!console->isVisible());
    }});

    // Add resource usage tracker UI panel
    auto resourceUsageTrackerID = Engine::getWindow()->addPanel(new ResourceUsageTrackerPanel{});
    InputManager::addCallback(InputKeyButton{Key::F1, InputKeyEventType::PRESSED, [resourceUsageTrackerID] {
        auto resourceUsageTracker = Engine::getWindow()->getPanel(resourceUsageTrackerID);
        resourceUsageTracker->setVisible(!resourceUsageTracker->isVisible());
    }});

    // Start script VM
    AngelScriptVM::init();

    // Create default resources
    Events::createEvent("chira::engine::create_default_resources");
    Events::update();
}

void Engine::run() {
    ImGui::SetCurrentContext(Engine::getWindow()->imguiContext);
    ImGui::GetIO().Fonts->Build();

    do {
        Engine::lastTime = Engine::currentTime;
        Engine::currentTime = glfwGetTime();

        Engine::window->update();
        Engine::window->render(glm::identity<glm::mat4>());

        glfwPollEvents();
        for (auto& keybind: InputManager::getKeyButtonCallbacks()) {
            if (glfwGetKey(Engine::window->window, static_cast<int>(keybind.getKey())) && keybind.getEventType() == InputKeyEventType::REPEAT)
                keybind();
        }
        for (auto& keybind: InputManager::getMouseButtonCallbacks()) {
            if (glfwGetMouseButton(Engine::window->window, static_cast<int>(keybind.getKey())) && keybind.getEventType() == InputKeyEventType::REPEAT)
                keybind();
        }

        if (DiscordRPC::initialized())
            DiscordRPC::updatePresence();
#ifdef CHIRA_USE_STEAMWORKS
        if (SteamAPI::Client::initialized())
            SteamAPI::Client::runCallbacks();
#endif
        Events::update();
    } while (!glfwWindowShouldClose(Engine::window->window));

    Logger::log(LOG_INFO_IMPORTANT, "Engine", TR("debug.engine.exit"));

    if (DiscordRPC::initialized())
        DiscordRPC::shutdown();
#ifdef CHIRA_USE_STEAMWORKS
    if (SteamAPI::Client::initialized())
        SteamAPI::Client::shutdown();
#endif

    Engine::window.reset();

    Resource::discardAll();

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

JSONSettingsLoader* Engine::getSettings() {
    return Engine::settingsLoader.get();
}

void Engine::setSettingsDefaults() {
    Engine::settingsLoader->setValue("max_point_lights", 64, false, false);
    Engine::settingsLoader->setValue("max_directional_lights", 4, false, false);
    Engine::settingsLoader->setValue("max_spot_lights", 4, false, false);
#ifdef CHIRA_USE_STEAMWORKS
    Engine::settingsLoader->setValue("steamworks", false, false, false);
#endif
    Engine::settingsLoader->setValue("window_width", 1600, false, false);
    Engine::settingsLoader->setValue("window_height", 900, false, false);
    Engine::settingsLoader->setValue("start_maximized", false, false, false);
    Engine::settingsLoader->setValue("fullscreen", false, false, false);
    Engine::settingsLoader->setValue("vsync", true, false, false);
    Engine::settingsLoader->setValue("raw_mouse_motion", true, false, false);
    Engine::settingsLoader->setValue("invert_y_axis", false, false, false);
    // todo: use computer language as default
    Engine::settingsLoader->setValue("language", std::string{"en"}, false, false);
    Engine::settingsLoader->save();
}

Window* Engine::getWindow() {
    return Engine::window.get();
}

bool Engine::isStarted() {
    return Engine::started;
}

double Engine::getDeltaTime() {
    return Engine::currentTime - Engine::lastTime;
}
