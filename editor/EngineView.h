#pragma once

#include <ui/FramePanel.h>

namespace chira {

class EngineView : public FramePanel {
public:    
    EngineView();
    void render() override;
};

} // namespace chira