#pragma once

#include <core/utility/UUIDGenerator.h>
#include <ui/IPanel.h>
#include <imfilebrowser.h>

namespace chira {

class Viewport;
class Scene;
class Entity;

class ControlsPanel : public IPanel {
public:
    explicit ControlsPanel(Viewport* viewport);

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
};

} // namespace chira
