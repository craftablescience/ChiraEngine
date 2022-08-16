#pragma once

#include <math/Types.h>
#include "IPanel.h"
#include <entity/Entity.h>
#include <entity/camera/EditorCamera.h>

namespace chira {

class Frame;

class FramePanel : public IPanel {
public:
    FramePanel(const std::string& title_, bool startVisible, ImVec2 windowSize, bool enforceSize = false);
    ~FramePanel() override;
    void renderContents() override;
    [[nodiscard]] Frame* getFrame() const;
    virtual void renderOverlayContents();
    virtual void renderPreFrameContents();
    virtual void renderPostFrameContents();
protected:
    Frame* frame = nullptr;
    glm::vec2i currentSize;
};

} // namespace chira
