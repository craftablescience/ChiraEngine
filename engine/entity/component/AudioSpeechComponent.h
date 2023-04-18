#pragma once

#include <string>
#include <soloud_speech.h>
#include <plugin/Audio.h>
#include "TransformComponent.h"

namespace chira {

struct AudioSpeechComponent {
    static constexpr auto in_place_delete = true;

    AudioSpeechComponent(std::string text_, bool autoplay_)
            : text(std::move(text_))
            , shouldPlay(autoplay_) {}

public:
    TransformComponent* transform = nullptr;
    std::string text;
    bool shouldPlay;

private:
    friend class Layer;

    SoLoud::Speech speech;
};

} // namespace chira
