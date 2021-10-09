#pragma once

#include "../../src/ui/abstractUiWindowComponent.h"
#include "../../src/utility/logger.h"
#include "../../src/resource/fontResource.h"

using namespace chira;

struct settingsUiWindowComponent : public abstractUiWindowComponent {
    explicit settingsUiWindowComponent(bool startVisible, const ImVec2& windowSize = ImVec2(0,0), bool enforceSize = false);
    void draw(double delta) override;
private:
    // Fields are initialized by values in settings loader
    int windowWidth = -1;
    int windowHeight = -1;
    bool startMaximized = false;
    bool fullscreen = false;
    bool rawMouseMotion = false;
    bool invertYAxis = false;
    std::string language;
    bool discordIntegration = false;
};
