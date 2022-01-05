#pragma once

#include "freecam.h"

namespace chira {
    class EditorCamera3d : public Freecam {
    public:
        EditorCamera3d(Entity* parent_, CameraProjectionMode mode, float fov_ = 90.0f, bool controls = true);
        EditorCamera3d(Entity* parent_, const std::string& name_, CameraProjectionMode mode, float fov_ = 90.0f, bool controls = true);
        explicit EditorCamera3d(CameraProjectionMode mode, float fov_ = 90.0f, bool controls = true);
        EditorCamera3d(const std::string& name_, CameraProjectionMode mode, float fov_ = 90.0f, bool controls = true);
        [[nodiscard]] float getScrollSpeedModifier() const {
            return this->scrollModifier;
        }
        void setScrollSpeedModifier(float scrollModifier_) {
            this->scrollModifier = scrollModifier_;
        }
    protected:
        float scrollModifier = 40.f;
        static void setupKeybinds();
    private:
        using Freecam::setupKeybinds;
    };
}
