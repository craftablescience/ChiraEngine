#include "EntitySelectPanel.h"

#include <entity/Layer.h>
#include "InspectorPanel.h"

using namespace chira;

EntitySelectPanel::EntitySelectPanel(Layer* layer_, InspectorPanel* inspector_)
        : IPanel("Entity Select", true)
        , layer(layer_)
        , inspector(inspector_) {}

void EntitySelectPanel::renderContents() {
    int id = 0;

    for (const auto& [sceneID, scene] : this->layer->getScenes()) {
        ImGui::PushID(++id);
        if (ImGui::CollapsingHeader(scene->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (const auto& [entityID, entity] : scene->getEntities()) {
                std::string buttonid = "X##";
                buttonid += entity->getName();
                if (ImGui::Button(buttonid.c_str())) {
                    scene->removeEntity(entityID);
                    break;
                }
                ImGui::SameLine();
                ImGui::PushID(++id);
                if (ImGui::Button(entity->getName().c_str())) {
                    this->inspector->setEntity(entity.get());
                }
                ImGui::PopID();
            }

            char entityName[512] {0};
            if (ImGui::InputText("Add Entity", entityName, sizeof(entityName), ImGuiInputTextFlags_EnterReturnsTrue) && entityName[0]) {
                scene->addEntity(entityName);
            }
        }
        ImGui::PopID();
    }

    ImGui::Separator();

    char sceneName[512] {0};
    if (ImGui::InputText("Add Scene", sceneName, sizeof(sceneName), ImGuiInputTextFlags_EnterReturnsTrue) && sceneName[0]) {
        this->layer->addScene(sceneName);
    }
}

void EntitySelectPanel::setLayer(Layer* layer_) {
    this->layer = layer_;
}

Layer* EntitySelectPanel::getLayer() const {
    return this->layer;
}
