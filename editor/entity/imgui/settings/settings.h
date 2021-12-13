#pragma once

#include <utility/logger.h>
#include <resource/fontResource.h>
#include <entity/imgui/window.h>

using namespace chira;

class settings : public window {
public:
    explicit settings(const ImVec2& windowSize = ImVec2(0,0));
    void renderContents() override;
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