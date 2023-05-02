#pragma once

#include <entity/Entity.h>
#include <ui/IPanel.h>

namespace chira {

class InspectorPanel : public IPanel {
public:
    InspectorPanel();

    void renderContents() override;

    void setSelected(Entity* selected_);

private:
    Entity* selected;
};

} // namespace chira
