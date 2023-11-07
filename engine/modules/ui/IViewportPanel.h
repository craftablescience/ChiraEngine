#pragma once

#include <core/math/Types.h>
#include "IPanel.h"

namespace chira {

class Viewport;

class IViewportPanel : public IPanel {
public:
    IViewportPanel(const std::string& title_, Viewport* viewport_, bool resizeLayer, bool startVisible, ImVec2 windowSize = {}, bool enforceSize = false);
    void renderContents() final;
    virtual void renderViewportContents() = 0;
protected:
    Viewport* viewport;
    glm::vec2i currentSize;
    bool shouldResize;
};

} // namespace chira
