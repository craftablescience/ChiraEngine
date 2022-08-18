#include "ResourceBrowser.h"

#include <fstream>
#include <core/Engine.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <i18n/TranslationManager.h>
#include <ui/IPanel.h>
#include <ui/FramePanel.h>
#include <render/texture/Texture.h>
#include <Editor.h>

using namespace chira;

CHIRA_CREATE_LOG(RESB);

ResourceBrowser::ResourceBrowser(MainEditorPanel* frame)
    : IPanel(TRC("ui.resourcebrowser.title"), true, ImVec2(2.0F, 2.0F), false), currentSize(2.0F, 2.0F) {
        this->mainpanel = frame;
}

void ResourceBrowser::loadResourceFolder(std::string resourceFolder) {
    // Resource related things
    auto resourceFolderPath = FilesystemResourceProvider::getResourceFolderPath(FILESYSTEM_ROOT_FOLDER + "/" + resourceFolder);
    if (resourceFolderPath.empty()) {
        LOG_RESB.error("Attempted to load resource folder " + resourceFolder + " but it doesn't exist!");
        return;
    }
    
    loadedResources.meshes = FilesystemResourceProvider::getDirectoryContents(FILESYSTEM_ROOT_FOLDER + "/" + resourceFolder + "/meshes/", "mdef");
    
    LOG_RESB.info("Finished searching for resources in " + resourceFolder);
}

// Thumbnail file element for resource browser
void ResourceBrowser::thumbnailFile(std::string fileName, FileType fileIcon = FILE_GENERIC) {
    ImVec2 startCurPos = ImGui::GetCursorPos();
        
    // Create button
    std::string fuckyouclang = "##" + val.fileName;
    ImGui::Button(fuckyouclang.c_str(), ImVec2(90.0F, 99.0F));
    
    // Putting the logic here to make sure every button works
    if (ImGui::IsItemActivated()) {
        this->mainpanel->setLoadedFile("file://meshes/" + val.fileName + ".mdef");
    }
    
    // Save this for later
    ImVec2 endCurPos = ImGui::GetCursorPos();
    // move our cursor position
    ImGui::SetCursorPos(startCurPos);
    // Create the rest of the file button
    switch(val.fileType) {
        using enum FileType;
        case FILE_GENERIC: {
            break;
        }
        case FILE_SCRIPT: {
            break;
        }
        case FILE_AUDIO: {
            break;
        }
        case FILE_TEXTURE: {
            break;
        }
        case FILE_MODEL: {
            break;
        }
        case FILE_MATERIAL: {
            break;
        }
        case FILE_IMAGE: {
            break;
        }
        case FILE_FONT: {
            break;
        }
        case FILE_MESH: {
            auto icon = Resource::getResource<Texture>("file://textures/ui/icons/cube.json");
            ImGui::Image((void*)icon->getHandle(), ImVec2(64.0F, 64.0F));
            break;
        }
    }
    ImGui::Text("%s", val.fileName.c_str());
    // Reset our cursor position back
    ImGui::SetCursorPos(endCurPos);
}

void ResourceBrowser::renderContents() {
    // display every mesh
    for ( auto const& [key, val] : loadedResources.meshes ) {
        this->thumbnailFile(val.fileName, val.fileType);
    }
}
