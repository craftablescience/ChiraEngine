#pragma once

#include <entity/Entity.h>
#include <ui/IPanel.h>

namespace chira {

class InspectorPanel : public IPanel {
public:
    InspectorPanel();

    void renderContents() override;

    void setEntity(Entity* newEnt);
    [[nodiscard]] Entity* getEntity() const;

private:
    Entity* curEnt;
};

} // namespace chira
