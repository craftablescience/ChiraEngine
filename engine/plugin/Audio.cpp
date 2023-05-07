#include "Audio.h"

#include <config/ConEntry.h>
#include "Plugin.h"

using namespace chira;

SoLoud::Soloud g_SoundEngine;
bool g_SoundEngineHasStarted = false;

struct AudioPlugin final : public IPlugin {
    void init() override {
        if (g_SoundEngineHasStarted)
            return;
        g_SoundEngine.init();
        g_SoundEngineHasStarted = true;
    }
    void deinit() override {
        g_SoundEngine.deinit();
    }
};
CHIRA_REGISTER_PLUGIN(AudioPlugin);

SoLoud::Soloud& Audio::get() {
    return g_SoundEngine;
}

[[maybe_unused]]
ConVar snd_volume{"snd_volume", 1.0, "Control the volume of the entire application.", CON_FLAG_CACHE, [](ConVar::CallbackArg arg) {
    if (g_SoundEngineHasStarted)
        return;
    g_SoundEngine.init();
    g_SoundEngineHasStarted = true;

    float volume = std::stof(arg.data());
    volume = std::clamp(volume, 0.0f, 1.0f);
    g_SoundEngine.setGlobalVolume(volume);
}};
