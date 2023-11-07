#pragma once

#include <string>
#include <string_view>

#include <soloud_noise.h>

#include <audio/Audio.h>
#include "TransformComponent.h"

namespace chira {

struct AudioNoiseComponent {
    static constexpr auto in_place_delete = true;

    explicit AudioNoiseComponent(SoLoud::Noise::NOISETYPES noise = SoLoud::Noise::NOISETYPES::WHITE)
            : noiseType(noise) {
        this->noise.setType(noise);
    }

    AudioNoiseComponent(float o0, float o1, float o2, float o3, float o4, float o5, float o6, float o7, float o8, float o9)
            : noiseType(static_cast<SoLoud::Noise::NOISETYPES>(-1)) {
        this->noise.setOctaveScale(o0, o1, o2, o3, o4, o5, o6, o7, o8, o9);
    }

    [[nodiscard]] SoLoud::Noise::NOISETYPES getNoiseType() const {
        return this->noiseType;
    }

    void setNoiseType(SoLoud::Noise::NOISETYPES noiseType_) {
        if (noiseType_ >= 0) {
            this->noise.setType(noiseType_);
        }
        this->noiseType = noiseType_;
    }

public:
    TransformComponent* transform = nullptr;
    SoLoud::Noise noise;

protected:
    /// Can be -1 to indicate a custom noise setup
    SoLoud::Noise::NOISETYPES noiseType;
};

} // namespace chira
