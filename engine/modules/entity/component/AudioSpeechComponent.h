#pragma once

#include <string>
#include <string_view>

#include <soloud_speech.h>

#include <audio/Audio.h>
#include "TransformComponent.h"

namespace chira {

struct AudioSpeechComponent {
    static constexpr auto in_place_delete = true;

    explicit AudioSpeechComponent(std::string_view speechText_ = "")
            : speechText(speechText_.data()) {
        this->speech.setText(this->speechText.c_str());
    }

    [[nodiscard]] std::string_view getSpeechText() const {
        return this->speechText;
    }

    void setSpeechText(std::string_view speechText_) {
        this->speechText = speechText_.data();
        this->speech.setText(this->speechText.c_str());
    }

public:
    TransformComponent* transform = nullptr;
    SoLoud::Speech speech;

protected:
    std::string speechText;
};

} // namespace chira
