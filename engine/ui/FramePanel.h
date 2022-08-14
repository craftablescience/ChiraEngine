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
    std::string_view addChild(chira::Entity *child);
    void removeChild(std::string_view name_);
    bool hasChild(std::string_view name_);
    Entity* getChild(std::string_view name_);
protected:
    Frame* frame = nullptr;
    glm::vec2i currentSize;
};

} // namespace chira
