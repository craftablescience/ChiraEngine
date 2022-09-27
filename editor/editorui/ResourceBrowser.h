#pragma once

#include <ui/IPanel.h>
#include <math/Types.h>
#include <entity/root/Group.h>
#include <map>
#include <resource/provider/FilesystemResourceProvider.h>
#include <Editor.h>
#include <render/texture/Texture.h>

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
    void changeDirectory(std::string path);
    
    resourceList loadedResources;
protected:
    // Paths
    std::string previousPath;
    std::string currentPath;
    // File list
    std::map<std::string, fileInfo> curdirList;
    // current resource folder to pull from
    std::string curResFolder;
    // List of resource folders we can pull from
    std::vector<std::string> resFolderList;

    glm::vec2i currentSize;
    MainEditorPanel* mainpanel;
};

} // namespace chira
