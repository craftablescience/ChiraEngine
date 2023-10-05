#include "Engine.h"

#include <config/ConEntry.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include <loader/mesh/OBJMeshLoader.h>
#include <loader/mesh/ChiraMeshLoader.h>
#include <module/Module.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <script/Lua.h>
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
    Device::queueDestroyWindow(Engine::getMainWindow(), true);
}};

[[maybe_unused]]
ConCommand crash{"crash", "Force-crashes the game or application (for debugging purposes).", [] {
    throw std::runtime_error{"Called crash command!"};
}, CON_FLAG_CHEAT};

ConVar win_width{"win_width", 1280, "The width of the main window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    if (Engine::getMainWindow()) {
        Device::setWindowSize(Engine::getMainWindow(), static_cast<int>(std::stoi(newValue.data())), Device::getWindowSize(Engine::getMainWindow()).y);
    }
}};

ConVar win_height{"win_height", 720, "The height of the main window.", CON_FLAG_CACHE, [](ConVar::CallbackArg newValue) {
    if (Engine::getMainWindow()) {
        Device::setWindowSize(Engine::getMainWindow(), Device::getWindowSize(Engine::getMainWindow()).x, static_cast<int>(std::stoi(newValue.data())));
    }
}};

void Engine::preinit(int argc, char* argv[]) {
#ifdef CHIRA_PLATFORM_WINDOWS
    // Enable colored text in Windows console by setting encoding to UTF-8
    // #define CP_UTF8 65001 in Windows.h
    system("chcp 65001 > nul");
#endif
    CommandLine::init(argc, argv);
    Resource::addResourceProvider(new FilesystemResourceProvider{ENGINE_FILESYSTEM_PATH});
    TranslationManager::addTranslationFile("file://i18n/engine");
    if (!ModuleRegistry::preinitAll()) [[unlikely]] {
        LOG_ENGINE.error("Failed to initialize modules! Make sure there are no circular dependencies.");
        exit(EXIT_FAILURE);
    }
}

void Engine::init(bool visibleSplashScreen /*= true*/) {
    Engine::started = true;

    if (!Device::initBackendAndCreateSplashscreen(visibleSplashScreen)) {
        LOG_ENGINE.error("Failed to initialize graphics!");
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    if (!Renderer::setupForDebugging()) {
        LOG_ENGINE.warning("Render backend \"{}\" failed to setup for debugging!", Renderer::getHumanName());
    }
#endif

    // Start up some auto-registered stuff (order is random!)
    ModuleRegistry::initAll();

    IMeshLoader::addMeshLoader("obj", new OBJMeshLoader{});
    IMeshLoader::addMeshLoader("cmdl", new ChiraMeshLoader{});

    // Create default resources
    Resource::createDefaultResources();

    // Start scripting VM
    Lua::init();

    Device::destroySplashscreen();

    Engine::mainWindow = Device::createWindow(win_width.getValue<int>(), win_height.getValue<int>(), TR("ui.window.title"), nullptr);
    if (!Engine::mainWindow) {
        LOG_ENGINE.error("Failed to create main window!");
        exit(EXIT_FAILURE);
    }

    // Add console UI panel
    auto consoleID = Device::addPanelToWindow(Engine::mainWindow, new ConsolePanel{});
    Input::KeyEvent::create(Input::Key::SDLK_BACKQUOTE, Input::KeyEventType::PRESSED, [consoleID] {
        auto console = Device::getPanelOnWindow(Engine::mainWindow, consoleID);
        console->setVisible(!console->isVisible());
    });

    // Add resource usage tracker UI panel
    auto resourceUsageTrackerID = Device::addPanelToWindow(Engine::mainWindow, new ResourceUsageTrackerPanel{});
    Input::KeyEvent::create(Input::Key::SDLK_F1, Input::KeyEventType::PRESSED, [resourceUsageTrackerID] {
        auto resourceUsageTracker = Device::getPanelOnWindow(Engine::mainWindow, resourceUsageTrackerID);
        resourceUsageTracker->setVisible(!resourceUsageTracker->isVisible());
    });
}

void Engine::run() {
    do {
        Engine::lastTime = Engine::currentTime;
        Engine::currentTime = Device::getTicks();

        Device::refreshWindows();

        ModuleRegistry::updateAll();

    } while (!Device::isWindowAboutToBeDestroyed(Engine::mainWindow));

    LOG_ENGINE.info("Exiting...");

    Device::destroyBackend();

    ModuleRegistry::deinitAll();

    Resource::discardAll();

    exit(EXIT_SUCCESS);
}

bool Engine::isStarted() {
    return Engine::started;
}

uint64_t Engine::getDeltaTicks() {
    return Engine::currentTime - Engine::lastTime;
}
