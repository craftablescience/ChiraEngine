#pragma once

#include <ui/FramePanel.h>

namespace chira {

class EngineView : public FramePanel {
public:    
    EngineView();
    void render() override;
    void renderOverlayContents() override;
    // variables
    std::string loadedFile;
    bool showGrid = true;
};

} // namespace chira