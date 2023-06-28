#include "Audio.h"

#include <config/ConEntry.h>
#include "Plugin.h"

using namespace chira;

CHIRA_CREATE_PLUGIN(Audio) {
    static inline const std::vector<std::string_view> DEPS;

    void init() override {
        if (this->started)
            return;
        this->soundEngine.init();
        this->started = true;
    }

    void deinit() override {
        this->soundEngine.deinit();
    }

public:
    SoLoud::Soloud soundEngine;
    bool started = false;
};
CHIRA_REGISTER_PLUGIN(Audio);

SoLoud::Soloud& Audio::get() {
    return g_Audio->soundEngine;
}

[[maybe_unused]]
ConVar snd_volume{"snd_volume", 1.0, "Control the volume of the entire application.", CON_FLAG_CACHE, [](ConVar::CallbackArg arg) {
    if (!g_Audio->started)
        return;
    float volume = std::stof(arg.data());
    volume = std::clamp(volume, 0.0f, 1.0f);
    Audio::get().setGlobalVolume(volume);
}};
