#pragma once

#include <ui/IPanel.h>
#include <math/Types.h>
namespace chira {

class ResourceBrowser : public IPanel {
public:
    ResourceBrowser();
    void renderContents() override;
protected:
    glm::vec2i currentSize;
};

} // namespace chira