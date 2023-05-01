#include "Runner.h"

#include <filesystem>
#include <fstream>

#include <core/Engine.h>
#include <core/Logger.h>
#include <core/Platform.h>
#include <render/backend/RenderDevice.h>

#ifdef CHIRA_PLATFORM_WINDOWS
    #include <windows.h>
#endif

using namespace chira;
namespace fs = std::filesystem;

CHIRA_CREATE_LOG(RUNNER);

// TODO: Figure out how to do dll stuff under Linux and macOS
// TODO: create a list of command line parameters that the runner uses then strip them out of the list before preinit

int main(int argc, const char* const argv[]) {
    // initialize the engine
    Engine::preInit(argc, argv);

    bool test_mode = false

    if (argc == 0) {
        LOG_RUNNER.error("No arguments provided! Quitting...");
        exit(EXIT_SUCCESS);
    }

    fs::path gamePath;

    for (int i = 0; i < argc; i++) {
        auto param = argv[i];

        COMMAND_LINE_PARAM("game") {
            gamePath = fs::path(argv[i + 1]);
        }
        COMMAND_LINE_PARAM("testmode") {
            // tells the runner to mark the window for deletion before executing Engine::Run()
            test_mode = true;
        }
    }

    // make sure we aren't about to load an invalid game folder
    if (!fs::exists(gamePath)) {
        LOG_RUNNER.error("Couldn't find gamePath '%s'", gamePath.string());
        exit(EXIT_FAILURE);
    }

    fs::path gameDLLPath{gamePath/"bin/game.dll"};
    if (!fs::exists(gameDLLPath)) {
        LOG_RUNNER.error("Couldn't find game DLL in '%s/bin'", gamePath.string());
        exit(EXIT_FAILURE);
    }

    // load the dll
#ifdef CHIRA_PLATFORM_WINDOWS
    HINSTANCE hinstgameLib;
    MYPROC gamePreInit, gameInit;
    bool fFreeResult = false;

    hinstgameLib = LoadLibrary(TEXT(gameDLLPath.c_str()));

    if (hinstgameLib != NULL) {
        gamePreInit = (MYPROC) GetProcAddress(hinstgameLib, "game_preInit");
        gameInit = (MYPROC) GetProcAddress(hinstgameLib, "game_init");
    
        if (NULL == gamePreInit) {
            LOG_RUNNER.error("FATAL: Unable to locate game_preInit in game DLL!");
            exit(2);
        }

        if (NULL == gameInit) {
            LOG_RUNNER.error("FATAL: Unable to locate game_init in game DLL!");
            exit(2);
        }
    }
#endif

    // TODO: implement gameinfo stuff for auto adding resources and the like automatically

#ifdef CHIRA_PLATFORM_WINDOWS
    (gamePreInit);
#endif

    Engine::Init();

#ifdef CHIRA_PLATFORM_WINDOWS
    (gameInit);
#endif

    // TODO: we should really handle most of this within the test game shouldn't we.
    if (test_mode)
        Device::queueDestroyWindow(Engine::getMainWindow(), true);
    Engine::Run();

#ifdef CHIRA_PLATFORM_WINDOWS
    // free dll
    fFreeResult = FreeLibrary(hinstgameLib);
    if (!fFreeResult)
        // this should never happen. If this happens that means we most likely
        // failed to load the game DLL which would trigger an error much earlier.
        printf("[RUNNER] Failed to free game module. This should not be possible?");
#endif
}