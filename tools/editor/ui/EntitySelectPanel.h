#pragma once

#include <ui/IPanel.h>

namespace chira {

class ControlsPanel;
class InspectorPanel;
class Viewport;

class EntitySelectPanel : public IPanel {
public:
    EntitySelectPanel(Viewport* layer_, ControlsPanel* controls_, InspectorPanel* inspector_);

    void renderContents() override;

    void setViewport(Viewport* viewport_);
    [[nodiscard]] Viewport* getViewport() const;

private:
    Viewport* viewport;
    ControlsPanel* controls;
    InspectorPanel* inspector;
};

} // namespace chira
