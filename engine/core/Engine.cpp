#include "Engine.h"

#include <config/ConEntry.h>
#include <entity/light/LightManager.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include <loader/mesh/OBJMeshLoader.h>
#include <loader/mesh/ChiraMeshLoader.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <script/AngelScriptVM.h>
#include <ui/debug/ConsolePanel.h>
#include <ui/debug/ResourceUsageTrackerPanel.h>
#include "CommandLine.h"

#ifdef DEBUG
    #include <render/backend/Renderer.h>
#endif
#ifdef CHIRA_USE_DISCORD
    #include <hook/DiscordRPC.h>
#endif
#ifdef CHIRA_USE_STEAMWORKS
    #include <hook/SteamAPI.h>
#endif

using namespace chira;

CHIRA_CREATE_LOG(ENGINE);

[[maybe_unused]]
static ConCommand quit{"quit", "Quits the game or application.", [] { // NOLINT(cert-err58-cpp)
    Engine::getWindow()->shouldStopAfterThisFrame(true);
}};

[[maybe_unused]]
static ConCommand crash{"crash", "Force-crashes the game or application (for debugging purposes).", [] { // NOLINT(cert-err58-cpp)
    throw std::runtime_error{"Called crash command!"};
}, CON_FLAG_CHEAT};

void Engine::preInit(int argc, const char* const argv[]) {
#ifdef _WIN32
    // Enable colored text in Windows console by setting encoding to UTF-8
    // #define CP_UTF8 65001 in windows.h
    system("chcp 65001 > nul");
#endif
    CommandLine::initialize(argc, argv);
    Resource::addResourceProvider(new FilesystemResourceProvider{ENGINE_FILESYSTEM_PATH});
    TranslationManager::addTranslationFile("file://i18n/engine");
    LightManager::setupShaderMacros();
}

void Engine::init(bool windowStartsVisible) {
    Engine::started = true;

    if (!glfwInit()) {
        LOG_ENGINE.error("GLFW failed to initialize!");
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback([](int error, const char* description) {
        LOG_ENGINE.error(fmt::format("GLFW Error {}: {}\n", error, description));
    });

    Engine::window.reset(new Window{TR("ui.window.title")});
    Engine::window->setVisible(windowStartsVisible);

#ifdef DEBUG
    if (!Renderer::setupForDebugging()) {
        LOG_ENGINE.warning(fmt::format("Render backend \"{}\" failed to setup for debugging!", Renderer::getHumanName()));
    }
#endif

    Engine::window->displaySplashScreen();

    IMeshLoader::addMeshLoader("obj", new OBJMeshLoader{});
    IMeshLoader::addMeshLoader("cmdl", new ChiraMeshLoader{});

#ifdef CHIRA_USE_STEAMWORKS
    bool steamEnable = ConVarRegistry::hasConVar("steam_enable") && ConVarRegistry::getConVar("steam_enable")->getValue<bool>();
    if (steamEnable && (!SteamAPI::Client::initialized() && !SteamAPI::Client::initSteam()))
        LOG_ENGINE.warning("Steam failed to initialize");
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

#ifdef CHIRA_USE_DISCORD
        if (DiscordRPC::initialized())
            DiscordRPC::updatePresence();
#endif
#ifdef CHIRA_USE_STEAMWORKS
        if (SteamAPI::Client::initialized())
            SteamAPI::Client::runCallbacks();
#endif
        Events::update();
    } while (!glfwWindowShouldClose(Engine::window->window));

    LOG_ENGINE.info("Exiting...");

#ifdef CHIRA_USE_DISCORD
    if (DiscordRPC::initialized())
        DiscordRPC::shutdown();
#endif
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
