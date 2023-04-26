#pragma once

#include <string>
#include <soloud_noise.h>
#include <plugin/Audio.h>
#include "TransformComponent.h"

namespace chira {

struct AudioNoiseComponent {
    static constexpr auto in_place_delete = true;

    explicit AudioNoiseComponent(SoLoud::Noise::NOISETYPES noise = SoLoud::Noise::NOISETYPES::WHITE) {
        this->noise.setType(noise);
    }

    AudioNoiseComponent(float o0, float o1, float o2, float o3, float o4, float o5, float o6, float o7, float o8, float o9) {
        this->noise.setOctaveScale(o0, o1, o2, o3, o4, o5, o6, o7, o8, o9);
    }

public:
    TransformComponent* transform = nullptr;
    SoLoud::Noise noise;
};

} // namespace chira
