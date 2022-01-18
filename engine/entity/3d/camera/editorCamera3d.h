#pragma once

#include "freecam.h"

namespace chira {
    class EditorCamera3d : public Freecam {
    public:
        EditorCamera3d(Entity* parent_, CameraProjectionMode mode, float fov_ = 90.f) : Freecam(parent_, mode, fov_) {}
        EditorCamera3d(Entity* parent_, const std::string& name_, CameraProjectionMode mode, float fov_ = 90.f) : Freecam(parent_, name_, mode, fov_) {}
        explicit EditorCamera3d(CameraProjectionMode mode, float fov_ = 90.f) : Freecam(mode, fov_) {}
        EditorCamera3d(const std::string& name_, CameraProjectionMode mode, float fov_ = 90.f) : Freecam(name_, mode, fov_) {}
        [[nodiscard]] float getScrollSpeedModifier() const {
            return this->scrollModifier;
        }
        void setScrollSpeedModifier(float scrollModifier_) {
            this->scrollModifier = scrollModifier_;
        }

        static void setupKeybinds();
    protected:
        float scrollModifier = 40.f;
    private:
        using Freecam::setupKeybinds;
    };
}
