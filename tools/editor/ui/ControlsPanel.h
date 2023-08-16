#pragma once

#include <ui/IPanel.h>
#include <utility/UUIDGenerator.h>
#include <imfilebrowser.h>

namespace chira {

class Viewport;
class Scene;
class Entity;

class ControlsPanel : public IPanel {
public:
    explicit ControlsPanel(Viewport* viewport);

    /// Opens a file dialog used to select a resource folder
    void addResourceFolderSelected();

    void preRenderContents() override;
    void renderContents() override;

    void setSelectedEntity(Entity* selected);
    void setSelectedScene(Scene* selected);

    [[nodiscard]] const Scene* getEditorScene() const {
        return this->scene;
    }
    [[nodiscard]] Scene* getEditorScene() {
        return this->scene;
    }

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
