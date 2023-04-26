#include "EntitySelectPanel.h"

#include <entity/Layer.h>
#include "InspectorPanel.h"

using namespace chira;

EntitySelectPanel::EntitySelectPanel(Layer* layer_, InspectorPanel* inspector_)
        : IPanel("Entity Select", true)
        , layer(layer_)
        , inspector(inspector_) {}

void EntitySelectPanel::renderContents() {
    for (const auto& [sceneID, scene] : this->layer->getScenes()) {
        if (ImGui::CollapsingHeader(scene->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (const auto& [entityID, entity] : scene->getEntities()) {
                if (ImGui::Button(entity->getName().c_str())) {
                    this->inspector->setEntity(entity.get());
                }
            }
        }
    }
}

void EntitySelectPanel::setLayer(Layer* layer_) {
    this->layer = layer_;
}

Layer* EntitySelectPanel::getLayer() const {
    return this->layer;
}
