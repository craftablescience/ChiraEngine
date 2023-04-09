#include "Engine.h"

#include <SDL.h>

#include <config/ConEntry.h>
#include <entity/light/LightManager.h>
#include <event/Events.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include <loader/mesh/OBJMeshLoader.h>
#include <loader/mesh/ChiraMeshLoader.h>
#include <plugin/Plugin.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <script/AngelScriptVM.h>
#include <ui/debug/ConsolePanel.h>
#include <ui/debug/ResourceUsageTrackerPanel.h>
#include "CommandLine.h"
#include "Platform.h"

#ifdef DEBUG
    #include <render/backend/RenderBackend.h>
#endif

using namespace chira;

CHIRA_CREATE_LOG(ENGINE);

[[maybe_unused]]
ConCommand quit{"quit", "Quits the game or application.", [] {
    Renderer::queueDestroyWindow(Engine::getDevice(), true);
}};

[[maybe_unused]]
ConCommand crash{"crash", "Force-crashes the game or application (for debugging purposes).", [] {
    throw std::runtime_error{"Called crash command!"};
}, CON_FLAG_CHEAT};

ConVar win_width{"win_width", 1280, "The width of the main window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    if (Engine::getDevice()) {
        Renderer::setWindowSize(Engine::getDevice(), static_cast<int>(std::stoi(newValue.data())), Renderer::getWindowSize(Engine::getDevice()).y);
    }
}};

ConVar win_height{"win_height", 720, "The height of the main window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    if (Engine::getDevice()) {
        Renderer::setWindowSize(Engine::getDevice(), Renderer::getWindowSize(Engine::getDevice()).x, static_cast<int>(std::stoi(newValue.data())));
    }
}};

void Engine::preInit(int argc, const char* const argv[]) {
#ifdef CHIRA_PLATFORM_WINDOWS
    // Enable colored text in Windows console by setting encoding to UTF-8
    // #define CP_UTF8 65001 in Windows.h
    system("chcp 65001 > nul");

    // Force enable DPI awareness because the manifest method didn't work
    SDL_SetHintWithPriority(SDL_HINT_WINDOWS_DPI_SCALING, "0", SDL_HINT_OVERRIDE);
    SDL_SetHintWithPriority(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2", SDL_HINT_OVERRIDE);
#endif
    CommandLine::init(argc, argv);
    Resource::addResourceProvider(new FilesystemResourceProvider{ENGINE_FILESYSTEM_PATH});
    TranslationManager::addTranslationFile("file://i18n/engine");
    PluginRegistry::preinitAll();
}

void Engine::init(bool visibleSplashScreen /*= true*/) {
    Engine::started = true;

    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER)) {
        LOG_ENGINE.error("SDL2 failed to initialize: {}", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (!Renderer::initBackendAndCreateSplashscreen(visibleSplashScreen)) {
        LOG_ENGINE.error("Failed to initialize graphics!");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    if (!Renderer::setupForDebugging()) {
        LOG_ENGINE.warning("Render backend \"{}\" failed to setup for debugging!", Renderer::getHumanName());
    }
#endif

    // Start up some auto-registered stuff (order is random!)
    PluginRegistry::initAll();

    IMeshLoader::addMeshLoader("obj", new OBJMeshLoader{});
    IMeshLoader::addMeshLoader("cmdl", new ChiraMeshLoader{});

    // Start script VM
    AngelScriptVM::init();

    // Create default resources
    Resource::createDefaultResources();

    // Any events fired?
    Events::update();

    Renderer::destroySplashscreen();

    Engine::mainWindow = Renderer::createWindow(win_width.getValue<int>(), win_height.getValue<int>(), TR("ui.window.title"), nullptr);

    // Add console UI panel
    auto consoleID = Renderer::addPanelToWindow(Engine::mainWindow, new ConsolePanel{});
    Input::KeyEvent::create(Input::Key::SDLK_BACKQUOTE, Input::KeyEventType::PRESSED, [consoleID] {
        auto console = Renderer::getPanelOnWindow(Engine::mainWindow, consoleID);
        console->setVisible(!console->isVisible());
    });

    // Add resource usage tracker UI panel
    auto resourceUsageTrackerID = Renderer::addPanelToWindow(Engine::mainWindow, new ResourceUsageTrackerPanel{});
    Input::KeyEvent::create(Input::Key::SDLK_F1, Input::KeyEventType::PRESSED, [resourceUsageTrackerID] {
        auto resourceUsageTracker = Renderer::getPanelOnWindow(Engine::mainWindow, resourceUsageTrackerID);
        resourceUsageTracker->setVisible(!resourceUsageTracker->isVisible());
    });
}

void Engine::run() {
    do {
        Engine::lastTime = Engine::currentTime;
        Engine::currentTime = SDL_GetTicks64();

        Renderer::refreshWindows();

        PluginRegistry::updateAll();

        Events::update();
    } while (!Renderer::isWindowAboutToBeDestroyed(Engine::mainWindow));

    LOG_ENGINE.info("Exiting...");

    PluginRegistry::deinitAll();

    Renderer::destroyBackend();

    Resource::discardAll();

    SDL_Quit();
    exit(EXIT_SUCCESS);
}

bool Engine::isStarted() {
    return Engine::started;
}

uint64_t Engine::getDeltaTicks() {
    return Engine::currentTime - Engine::lastTime;
}
