#pragma once

#include <entity/Entity.h>
#include <ui/IPanel.h>

namespace chira {

class InspectorPanel : public IPanel {
public:
    InspectorPanel();

    void renderContents() override;

    void setSelectedEntity(Entity* selected);
    void setSelectedScene(Scene* selected);

private:
    Entity* selectedEntity;
    Scene* selectedScene;

    void renderContentsForSelectedEntity();
    void renderContentsForSelectedScene();
};

} // namespace chira
