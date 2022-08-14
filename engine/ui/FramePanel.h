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
    std::string_view addChild(Entity* child);
    void removeChild(std::string_view name_);
    bool hasChild(std::string_view name_);
    Entity* getChild(std::string_view name_);
    void setCamera(Camera* camera);
protected:
    Frame* frame = nullptr;
    glm::vec2i currentSize;
};

} // namespace chira
