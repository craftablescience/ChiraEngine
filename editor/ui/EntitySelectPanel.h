#pragma once

#include <ui/IPanel.h>

namespace chira {

class ModelViewerPanel;
class InspectorPanel;
class Layer;

class EntitySelectPanel : public IPanel {
public:
    EntitySelectPanel(Layer* layer_, ModelViewerPanel* editor_, InspectorPanel* inspector_);

    void renderContents() override;

    void setLayer(Layer* layer_);
    [[nodiscard]] Layer* getLayer() const;

private:
    Layer* layer;
    ModelViewerPanel* editor;
    InspectorPanel* inspector;
};

} // namespace chira
