#pragma once

#include "../EngineView.h"

namespace chira {

class ModelScene : public EngineView {
public:    
    ModelScene();
    void renderOverlayContents() override;
    std::string loadedFile;
private:
    bool showGrid = true;
};

} // namespace chira