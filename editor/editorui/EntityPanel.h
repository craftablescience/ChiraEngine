#pragma once

#include <ui/IPanel.h>
#include <math/Types.h>
namespace chira {

class EntityPanel : public IPanel {
public:
    EntityPanel(Frame* frame);
    void renderContents() override;
protected:
    glm::vec2i currentSize;
    Frame* curframe;
};

} // namespace chira
