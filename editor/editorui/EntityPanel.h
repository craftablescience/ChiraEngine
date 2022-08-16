#pragma once

#include <ui/IPanel.h>
#include <math/Types.h>
namespace chira {

class EntityPanel : public IPanel {
public:
    EntityPanel();
    void renderContents() override;
protected:
    glm::vec2i currentSize;
};

} // namespace chira