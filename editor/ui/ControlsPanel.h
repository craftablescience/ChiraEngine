#pragma once

#include <ui/IPanel.h>
#include <utility/UUIDGenerator.h>
#include <imfilebrowser.h>

namespace chira {

class Layer;
class Scene;
class Entity;

class ControlsPanel : public IPanel {
public:
    explicit ControlsPanel(Layer* layer);

    /// Opens a file dialog used to select a resource folder
    void addResourceFolderSelected();

    void preRenderContents() override;
    void renderContents() override;

    void setSelectedEntity(Entity* selected);
    void setSelectedScene(Scene* selected);

private:
    Scene* scene;
    Entity* selectedEntity;
    Scene* selectedScene;
    std::string loadedFile;
    uuids::uuid gridID;
    uuids::uuid previewID;
    bool showGrid = true;
    ImGui::FileBrowser folderDialog{ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_SelectDirectory};
};

} // namespace chira
