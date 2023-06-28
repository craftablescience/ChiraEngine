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

#include "../ToolHelpers.h"

using namespace chira;

CHIRA_SETUP_GUI_TOOL(EDITOR);

#ifdef CHIRA_USE_DISCORD
CHIRA_GET_PLUGIN(Discord);
#endif

int main(int argc, const char* const argv[]) {
    Engine::preInit(argc, argv);
    Resource::addResourceProvider(new FilesystemResourceProvider{"editor"});
    TranslationManager::addTranslationFile("file://i18n/editor");
    TranslationManager::addUniversalFile("file://i18n/editor");

#ifdef CHIRA_USE_DISCORD
    g_Discord->init(TR("editor.discord.application_id"));
    g_Discord->setLargeImage("main_logo");
    g_Discord->setTopButton({"View on GitHub", "https://github.com/craftablescience/ChiraEngine"});
    g_Discord->setBottomButton({"Join Discord", "https://discord.gg/ASgHFkX"});
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
