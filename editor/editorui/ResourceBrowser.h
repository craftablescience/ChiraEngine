#pragma once

#include <ui/IPanel.h>
#include <math/Types.h>
#include <entity/root/Group.h>

namespace chira {

class ResourceBrowser : public IPanel {
public:
    ResourceBrowser();
    void renderContents() override;
    void GetMeshList(std::string meshesPath);
    Group MeshList;
protected:
    glm::vec2i currentSize;
};

} // namespace chira