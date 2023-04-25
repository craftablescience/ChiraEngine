#pragma once

#include <ui/IPanel.h>
#include <entity/Entity.h>

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
