#include "Engine.h"

#include <glad/gl.h>
#include <backends/imgui_impl_opengl3.h>

#include <config/Config.h>
#include <config/ConEntry.h>
#include <hook/DiscordRPC.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include <loader/mesh/OBJMeshLoader.h>
#include <loader/mesh/ChiraMeshLoader.h>
#include <render/Light.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <resource/ShaderResource.h>
#include <script/AngelScriptVM.h>
#include <ui/debug/ConsolePanel.h>
#include <ui/debug/ResourceUsageTrackerPanel.h>

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

void Engine::preInit() {
    Resource::addResourceProvider(new FilesystemResourceProvider{ENGINE_FILESYSTEM_PATH});
    TranslationManager::addTranslationFile("file://i18n/engine");
}

void Engine::init(bool windowStartsVisible) {
    Engine::started = true;

    if (!glfwInit()) {
        Logger::log(LogType::LOG_ERROR, "GLFW", "GLFW failed to initialize!");
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback([](int error, const char* description) {
        Logger::log(LogType::LOG_ERROR, "GLFW", fmt::format("GLFW Error {}: {}\n", error, description));
    });

    Engine::window.reset(new Window{TR("ui.window.title")});
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
                Logger::log(LogType::LOG_ERROR, "OpenGL", output);
            else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
                Logger::log(LogType::LOG_INFO, "OpenGL", output);
            else
                // Logging as a warning because most of the time the program runs perfectly fine
                Logger::log(LogType::LOG_WARNING, "OpenGL", output);
        }, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    IMGUI_CHECKVERSION();
#endif

    Engine::window->displaySplashScreen();

    IMeshLoader::addMeshLoader("obj", new OBJMeshLoader{});
    IMeshLoader::addMeshLoader("cmdl", new ChiraMeshLoader{});

    // todo: move this to a general lighting manager
    if (ConVarRegistry::hasConVar("max_point_lights")) {
        ShaderResource::addPreprocessorSymbol("MAX_POINT_LIGHTS", ConVarRegistry::getConVar("max_point_lights")->getValue<std::string>());
    }
    if (ConVarRegistry::hasConVar("max_directional_lights")) {
        ShaderResource::addPreprocessorSymbol("MAX_DIRECTIONAL_LIGHTS", ConVarRegistry::getConVar("max_directional_lights")->getValue<std::string>());
    }
    if (ConVarRegistry::hasConVar("max_spot_lights")) {
        ShaderResource::addPreprocessorSymbol("MAX_SPOT_LIGHTS", ConVarRegistry::getConVar("max_spot_lights")->getValue<std::string>());
    }

#ifdef CHIRA_USE_STEAMWORKS
    bool steamEnable = ConVarRegistry::hasConVar("steam_enable") && ConVarRegistry::getConVar("steam_enable")->getValue<bool>();
    if (steamEnable && (!SteamAPI::Client::initialized() && !SteamAPI::Client::initSteam()))
        Logger::log(LogType::LOG_ERROR, "Steam", TR("error.steam.initialization_failure"));
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

    Logger::log(LogType::LOG_INFO, "Engine", "Exiting...");

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

Window* Engine::getWindow() {
    return Engine::window.get();
}

bool Engine::isStarted() {
    return Engine::started;
}

double Engine::getDeltaTime() {
    return Engine::currentTime - Engine::lastTime;
}
