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
    bool thumbnailFile(std::string fileName, std::string fileIcon);
    
    //Group MeshList = MeshList("RB_MeshList");
protected:
    glm::vec2i currentSize;
};

} // namespace chira
