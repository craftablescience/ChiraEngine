#pragma once

#include <math/Types.h>
#include "IPanel.h"

namespace chira {

class Layer;

class ILayerPanel : public IPanel {
public:
    ILayerPanel(const std::string& title_, Layer* layer_, bool resizeLayer, bool startVisible, ImVec2 windowSize = {}, bool enforceSize = false);
    void renderContents() final;
    virtual void renderLayerContents() = 0;
protected:
    Layer* layer;
    glm::vec2i currentSize;
    bool shouldResize;
};

} // namespace chira
