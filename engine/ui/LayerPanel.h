#pragma once

#include <math/Types.h>
#include "IPanel.h"

namespace chira {

class Layer;

class LayerPanel : public IPanel {
public:
    LayerPanel(const std::string& title_, Layer* layer_, bool startVisible, ImVec2 windowSize, bool enforceSize = false);
    void renderContents() override;
protected:
    Layer* layer;
    glm::vec2i currentSize;
};

} // namespace chira
