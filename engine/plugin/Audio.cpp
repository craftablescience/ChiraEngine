#include "Audio.h"

#include "Plugin.h"

using namespace chira;

SoLoud::Soloud g_SoundEngine;

struct AudioPlugin final : public IPlugin {
    // Discord should be initialized manually before Engine::init
    void init() override {
        g_SoundEngine.init();
    }
    void deinit() override {
        g_SoundEngine.deinit();
    }
};
CHIRA_REGISTER_PLUGIN(AudioPlugin);

SoLoud::Soloud& Audio::get() {
    return g_SoundEngine;
}
