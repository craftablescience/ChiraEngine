#include "Audio.h"

#include <core/config/ConEntry.h>

using namespace chira;

CHIRA_REGISTER_MODULE(Audio);

[[maybe_unused]]
ConVar snd_volume{"snd_volume", 1.0, "Control the volume of the entire application.", CON_FLAG_CACHE, [](ConVar::CallbackArg arg) {
    if (!g_Audio->isInitialized())
        return;
    float volume = std::stof(arg.data());
    volume = std::clamp(volume, 0.0f, 1.0f);
    g_Audio->get().setGlobalVolume(volume);
}};

bool AudioModule::preinit() {
    this->soundEngine.init();
    this->soundEngine.setGlobalVolume(static_cast<float>(std::clamp(snd_volume.getValue<double>(), 0.0, 1.0)));
    return true;
}
