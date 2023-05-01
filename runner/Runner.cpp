#include "Runner.h"

#include <filesystem>
#include <fstream>

#include <core/Engine.h>
#include <core/Logger.h>
#include <core/Platform.h>

#ifdef CHIRA_PLATFORM_WINDOWS
    #include <windows.h>
#endif

using namespace chira;
namespace fs = std::filesystem;

CHIRA_CREATE_LOG(RUNNER);

int main(int argc, const char* const argv[]) {
    // initialize the engine
    Engine::preInit(argc, argv);

    if (argc == 0) {
        LOG_RUNNER.error("No arguments provided! Quitting...");
        Resource::DiscardAll();
        return 0;
    }

    fs::path gamePath;

    for (int i = 0; i < argc; i++) {
        auto param = argv[i];

        COMMAND_LINE_PARAM("game", {
            gamePath = fs::path(argv[i + 1]);
        });
    }

    // make sure we aren't about to load an invalid game folder
    if (!fs::exists(gamePath)) {
        LOG_RUNNER.error("Couldn't find gamePath '%s'", gamePath.string());
        Resource::DiscardAll();
        return 0;
    }

    fs::path gameDLLPath{gamePath/"bin/game.dll"};
    if (!fs::exists(gameDLLPath)) {
        LOG_RUNNER.error("Couldn't find game DLL in '%s/bin'", gamePath.string());
        Resource::DiscardAll();
        return 0;
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
            Resource::DiscardAll();
            return 0;
        }

        if (NULL == gameInit) {
            LOG_RUNNER.error("FATAL: Unable to locate game_init in game DLL!");
            Resource::DiscardAll();
            return 0;
        }
    }
#endif

    // TODO: implement gameinfo stuff for auto adding resources and the like

#ifdef CHIRA_PLATFORM_WINDOWS
    (gamePreInit);
#endif

    Engine::Init();

#ifdef CHIRA_PLATFORM_WINDOWS
    (gameInit);
#endif

    Engine::Run();

#ifdef CHIRA_PLATFORM_WINDOWS
    // free dll
    fFreeResult = FreeLibrary(hinstgameLib);
    if (!fFreeResult)
        printf("[RUNNER] Failed to free game libary? Something went horribly wrong!");
#endif
}