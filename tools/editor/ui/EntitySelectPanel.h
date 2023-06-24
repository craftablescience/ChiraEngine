#pragma once

#include <ui/IPanel.h>

namespace chira {

class ControlsPanel;
class InspectorPanel;
class Layer;

class EntitySelectPanel : public IPanel {
public:
    EntitySelectPanel(Layer* layer_, ControlsPanel* controls_, InspectorPanel* inspector_);

    void renderContents() override;

    void setLayer(Layer* layer_);
    [[nodiscard]] Layer* getLayer() const;

private:
    Layer* layer;
    ControlsPanel* controls;
    InspectorPanel* inspector;
};

} // namespace chira
