#pragma once

#include <math/Types.h>
#include "IPanel.h"

namespace chira {

class Frame;

class FramePanel : public IPanel {
public:
    FramePanel(const std::string& title_, bool startVisible, ImVec2 windowSize, bool enforceSize = false);
    ~FramePanel() override;
    void renderContents() override;
protected:
    Frame* frame = nullptr;
    glm::vec2i currentSize;
};

} // namespace chira
