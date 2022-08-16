#include "ResourceBrowser.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <core/Engine.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <i18n/TranslationManager.h>
#include <ui/IPanel.h>
#include <ui/FramePanel.h>
#include <utility/Dialogs.h>
// Used because file dialogs are broken on macOS
#include <imfilebrowser.h>

using namespace chira;

CHIRA_CREATE_LOG(RESB);

ResourceBrowser::ResourceBrowser()
    : IPanel(TRC("ui.resourcebrowser.title"), true, ImVec2(2.0F, 2.0F), false), currentSize(2.0F, 2.0F) {
}

void ResourceBrowser::loadResourceFolder(std::string resourceFolder) {
    // Resource related things
    auto resourceFolderPath = FilesystemResourceProvider::getResourceFolderPath(FILESYSTEM_ROOT_FOLDER + "/" + resourceFolder);
    if (resourceFolderPath.empty()) {
        LOG_RESB.error("Attempted to load resource folder " + resourceFolder + " but it doesn't exist!");
        return;
    }
    
    loadedResources.meshes = FilesystemResourceProvider::getDirectoryContents(FILESYSTEM_ROOT_FOLDER + "/" + resourceFolder + "/meshes", "mdef");
    
    LOG_RESB.info("Finished searching for resources in " + resourceFolder);
}

// Thumbnail file element for resource browser
bool ResourceBrowser::thumbnailFile(std::string fileName, std::string fileIcon = "FileGeneric") {
    auto buttonBool = false;
    ImVec2 startCurPos = ImGui::GetCursorPos();
        
    // Create button
    buttonBool = ImGui::Button("##file", ImVec2(90.0F, 99.0F));
    // Save this for later
    ImVec2 endCurPos = ImGui::GetCursorPos();
    // move our cursor position
    ImGui::SetCursorPos(startCurPos);
    // Create the rest of the file button
    ImGui::Text(fileName.c_str());
    
    // Reset our cursor position back
    ImGui::SetCursorPos(endCurPos);
    // Return the press result of the button
    return buttonBool;
}

void ResourceBrowser::renderContents() {
    // display every mesh
    for ( auto const& [key, val] : loadedResources.meshes ) {
        this->thumbnailFile(loadedResources.meshes[key].fileName);
    }
}
