#pragma once

#include <string>
#include <string_view>

#include <soloud_sfxr.h>

#include <audio/Audio.h>
#include <resource/BinaryResource.h>
#include "TransformComponent.h"

namespace chira {

struct AudioSfxrComponent {
    static constexpr auto in_place_delete = true;

    explicit AudioSfxrComponent(SoLoud::Sfxr::SFXR_PRESETS preset = SoLoud::Sfxr::SFXR_PRESETS::COIN, int seed_ = 0)
            : sfxrPreset(preset)
            , seed(seed_) {
        this->setSfxrFromPreset(this->sfxrPreset, this->seed);
    }

    explicit AudioSfxrComponent(std::string cfgId)
            : sfxrPreset(static_cast<SoLoud::Sfxr::SFXR_PRESETS>(-1))
            , seed(0)
            , cfgFile(std::move(cfgId)) {
        this->setSfxrFromConfig(this->cfgFile);
    }

    [[nodiscard]] SoLoud::Sfxr::SFXR_PRESETS getSfxrPreset() const {
        return this->sfxrPreset;
    }

    [[nodiscard]] int getSfxrSeed() const {
        return this->seed;
    }

    [[nodiscard]] std::string_view getSfxrConfigID() const {
        return this->cfgFile;
    }

    void setSfxrFromPreset(SoLoud::Sfxr::SFXR_PRESETS sfxrPreset_, int seed_ = 0) {
        if (sfxrPreset_ >= 0) {
            this->sfxr.resetParams();
            this->sfxr.loadPreset(sfxrPreset_, seed_);
        }
        this->sfxrPreset = sfxrPreset_;
        this->seed = seed_;
    }

    void setSfxrFromConfig(std::string cfgId) {
        this->cfgFile = std::move(cfgId);
        auto config = Resource::getUniqueUncachedResource<BinaryResource>(this->cfgFile);
        this->sfxr.resetParams();
        this->sfxr.loadParamsMem(const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(config->getBuffer())), static_cast<unsigned int>(config->getBufferLength()), true, true);
    }

public:
    TransformComponent* transform = nullptr;
    SoLoud::Sfxr sfxr;

protected:
    /// Can be -1 to indicate it's loaded from a config file
    SoLoud::Sfxr::SFXR_PRESETS sfxrPreset;
    int seed = 0;
    std::string cfgFile;
};

} // namespace chira
