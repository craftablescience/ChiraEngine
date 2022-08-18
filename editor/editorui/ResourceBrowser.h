#pragma once

#include <ui/IPanel.h>
#include <math/Types.h>
#include <entity/root/Group.h>
#include <map>
#include <resource/provider/FilesystemResourceProvider.h>
#include <Editor.h>

namespace chira {

struct resourceList {
    std::map<std::string, fileInfo> meshes;
    std::map<std::string, fileInfo> materials;
    std::map<std::string, fileInfo> fonts;
};

class ResourceBrowser : public IPanel {
public:
    ResourceBrowser(MainEditorPanel* frame);
    void renderContents() override;
    
    /// Resource list handling methods
    void loadResourceFolder(std::string resourceFolder);
    
    /// Resource Browser specific elements
    bool thumbnailFile(std::string fileName, FileType fileIcon);
    
    resourceList loadedResources;
protected:
    glm::vec2i currentSize;
    MainEditorPanel* mainpanel;
};

} // namespace chira
