#pragma once

#include <string>
#include "imgui.h"
#include "../../src/utility/logger.h"
#include "../../src/resource/fontResource.h"

struct settings {
public:
    settings();
    void render();
    void setEnabled(bool enabled);
    [[nodiscard]] bool getEnabled() const;
private:
    // Fields are initialized by values in settings loader
    bool isEnabled = false;
    int windowWidth = -1;
    int windowHeight = -1;
    bool startMaximized = false;
    bool fullscreen = false;
    bool rawMouseMotion = false;
    bool invertYAxis = false;
    std::string language;
    bool discordIntegration = false;
};
