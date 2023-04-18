#pragma once

#include <string>
#include <soloud_speech.h>
#include <plugin/Audio.h>
#include "TransformComponent.h"

namespace chira {

struct AudioSpeechComponent {
    static constexpr auto in_place_delete = true;

    AudioSpeechComponent() = default;

public:
    TransformComponent* transform = nullptr;
    SoLoud::Speech speech;
};

} // namespace chira
