#pragma once

#include "Freecam.h"

namespace chira {

class EditorCamera : public Freecam {
public:
    EditorCamera(std::string name_, CameraProjectionMode mode, float fov_ = 90.f) : Freecam(std::move(name_), mode, fov_) {}
    explicit EditorCamera(CameraProjectionMode mode, float fov_ = 90.f) : Freecam(mode, fov_) {}
    [[nodiscard]] float getScrollSpeedModifier() const {
        return this->scrollModifier;
    }
    void setScrollSpeedModifier(float scrollModifier_) {
        this->scrollModifier = scrollModifier_;
    }

    //todo(input): this is dumb
    static void setupKeybinds();
protected:
    float scrollModifier = 40.f;
private:
    using Freecam::setupKeybinds;
};

} // namespace chira
