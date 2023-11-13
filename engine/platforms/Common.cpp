#include "Common.h"

#include <core/config/ConEntry.h>
#include <core/CommandLine.h>
#include <core/IModule.h>
#include <core/Platform.h>
#include <i18n/TranslationManager.h>
#include <input/InputManager.h>
#include <loader/mesh/OBJMeshLoader.h>
#include <loader/mesh/ChiraMeshLoader.h>
#include <script/Lua.h>
#include <ui/debug/ConsolePanel.h>
#include <ui/debug/ResourceUsageTrackerPanel.h>

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

void Engine::init(int argc, const char* argv[], bool onlyInitEssentials) {
	if (Engine::started) {
		return;
	}
	Engine::started = true;

#ifdef CHIRA_PLATFORM_WINDOWS
    // Enable colored text in Windows console by setting encoding to UTF-8
    // #define CP_UTF8 65001 in Windows.h
    system("chcp 65001 > nul");
#endif
    CommandLine::init(argc, argv);

    if (!FileSystem::init()) [[unlikely]] {
		LOG_ENGINE.error("Failed to initialize filesystem!");
	    exit(EXIT_FAILURE);
	}
	const auto& projectConfig = FileSystem::getProjectConfig();

	// Necessary to have filesystem setup so it can read/write the config file
	ConEntryRegistry::initializeConVarCache();

	// todo: autoregister this
	TranslationManager::addTranslationFile("i18n/engine");

	if (!ModuleRegistry::initAll()) [[unlikely]] {
        LOG_ENGINE.error("Failed to initialize modules! Make sure there are no circular dependencies.");
        exit(EXIT_FAILURE);
    }

	// todo: make a factory
	IMeshLoader::addMeshLoader("obj", new OBJMeshLoader{});
	IMeshLoader::addMeshLoader("cmdl", new ChiraMeshLoader{});

	if (onlyInitEssentials) {
		// Bail early before creating the render device/backend or scripting VM
		// Note default resources will not be constructed either due to needing a render backend
		return;
	}

	if (!Device::initBackendAndCreateSplashscreen(projectConfig.splashscreen)) {
		LOG_ENGINE.error("Failed to initialize render device backend!");
		exit(EXIT_FAILURE);
	}

#ifdef DEBUG
	if (!Renderer::setupForDebugging()) {
		LOG_ENGINE.warning("Render API backend \"{}\" failed to setup for debugging!", Renderer::getHumanName());
	}
#endif

	// Create default resources
	Resource::createDefaultResources();

    // Start scripting VM
    Lua::init();

	Device::destroySplashscreen();

	Engine::mainWindow = Device::createWindow(win_width.getValue<int>(), win_height.getValue<int>(),
	                                          TR("ui.window.title"), nullptr);
	if (!Engine::mainWindow) [[unlikely]] {
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

	FileSystem::deinit();

    exit(EXIT_SUCCESS);
}

bool Engine::isStarted() {
    return Engine::started;
}

uint64_t Engine::getDeltaTicks() {
    return Engine::currentTime - Engine::lastTime;
}
