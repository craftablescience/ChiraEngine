#pragma once

#include <ui/IPanel.h>
#include <utility/UUIDGenerator.h>

#include <imfilebrowser.h>

namespace chira {

class Layer;
class Scene;
class Entity;

class ModelViewerPanel : public IPanel {
public:
    explicit ModelViewerPanel(Layer* layer);

    /// Opens a file dialog used to select a model definition
    void addModelSelected();

    /// Opens a file dialog used to select a resource folder
    void addResourceFolderSelected();

    void convertToModelTypeSelected(const std::string& filepath, const std::string& type) const;

    /// Opens a file dialog used to save a mesh to OBJ
    void convertToOBJSelected() const;

    /// Opens a file dialog used to save a mesh to CMDL
    void convertToCMDLSelected() const;

    void preRenderContents() override;
    void renderContents() override;

    void setLoadedFile(const std::string& meshName);

    [[nodiscard]] uuids::uuid getMeshId() const;

private:
    Scene* scene;
    Entity* grid;
    std::string loadedFile;
    uuids::uuid meshId;
    bool showGrid = true;
    ImGui::FileBrowser modelDialog{ImGuiFileBrowserFlags_CloseOnEsc};
    ImGui::FileBrowser folderDialog{ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_SelectDirectory};
    ImGui::FileBrowser saveDialogOBJ{ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_EnterNewFilename};
    ImGui::FileBrowser saveDialogCMDL{ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_EnterNewFilename};
};

} // namespace chira
