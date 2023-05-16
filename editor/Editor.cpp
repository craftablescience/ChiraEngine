// Disable console window on Windows (MSVC)
#include <core/Platform.h>
#if defined(CHIRA_PLATFORM_WINDOWS) && !defined(DEBUG) && defined(CHIRA_COMPILER_MSVC)
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

// Use the best available GPU on Windows
#if defined(CHIRA_PLATFORM_WINDOWS)
extern "C" {
[[maybe_unused]] __declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
[[maybe_unused]] __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
}
#endif

#include <config/ConEntry.h>
#include <core/Engine.h>
#include <i18n/TranslationManager.h>
#include <resource/provider/FilesystemResourceProvider.h>

#ifdef CHIRA_USE_DISCORD
    #include <plugin/DiscordRPC.h>
#endif
#ifdef CHIRA_USE_STEAMWORKS
    #include <plugin/SteamAPI.h>
#endif

// Need to register phong material!
#include <render/material/MaterialPhong.h>

#include "ui/ControlsPanel.h"
#include "ui/EntitySelectPanel.h"
#include "ui/InspectorPanel.h"
#include "ui/ScriptEditorPanel.h"

using namespace chira;

int main(int argc, const char* const argv[]) {
    Engine::preInit(argc, argv);
    Resource::addResourceProvider(new FilesystemResourceProvider{"editor"});
    TranslationManager::addTranslationFile("file://i18n/editor");
    TranslationManager::addUniversalFile("file://i18n/editor");

#ifdef CHIRA_USE_DISCORD
    if (auto* discord_enable = ConEntryRegistry::getConVar("discord_enable"); discord_enable && discord_enable->getValue<bool>()) {
        DiscordRPC::init(TR("editor.discord.application_id"));
        DiscordRPC::setLargeImage("main_logo");
        DiscordRPC::setTopButton({"Join Discord", "https://discord.gg/ASgHFkX"});
    }
#endif

#if defined(CHIRA_USE_STEAMWORKS) && defined(DEBUG)
    if (auto* steam_enable = ConEntryRegistry::getConVar("steam_enable"); steam_enable && steam_enable->getValue<bool>()) {
        // Steam API docs say this is bad practice, I say I don't care
        SteamAPI::generateAppIDFile(1728950);
    }
#endif

    Engine::init();

    auto* window = Engine::getMainWindow();
    auto* layer = Device::getWindowLayer(window);
    layer->setBackgroundColor({0.15f});

    auto controls = new ControlsPanel{layer};
    Device::addPanelToWindow(window, controls);

    auto inspector = new InspectorPanel{};
    Device::addPanelToWindow(window, inspector);

    auto scriptEditor = new ScriptEditorPanel{};
    Device::addPanelToWindow(window, scriptEditor);

    Device::addPanelToWindow(window, new EntitySelectPanel{layer, controls, inspector});

    Engine::run();
}
