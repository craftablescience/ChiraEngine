#pragma once

#include <string>
#include <soloud_sfxr.h>
#include <plugin/Audio.h>
#include <resource/BinaryResource.h>
#include "TransformComponent.h"

namespace chira {

struct AudioSfxrComponent {
    static constexpr auto in_place_delete = true;

    explicit AudioSfxrComponent(int preset, int seed = 0) {
        this->sfxr.loadPreset(preset, seed);
    }

    explicit AudioSfxrComponent(const std::string& cfgId) {
        auto config = Resource::getUniqueUncachedResource<BinaryResource>(cfgId);
        this->sfxr.loadParamsMem(const_cast<unsigned char*>(config->getBuffer()), config->getBufferLength(), true, true);
    }

public:
    TransformComponent* transform = nullptr;
    SoLoud::Sfxr sfxr;
};

} // namespace chira
