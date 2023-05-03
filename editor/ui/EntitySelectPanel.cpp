#include "EntitySelectPanel.h"

#include <entity/Layer.h>
#include "ControlsPanel.h"
#include "InspectorPanel.h"

using namespace chira;

EntitySelectPanel::EntitySelectPanel(Layer* layer_, ControlsPanel* controls_, InspectorPanel* inspector_)
        : IPanel("Entity Select", true)
        , layer(layer_)
        , controls(controls_)
        , inspector(inspector_) {}

void EntitySelectPanel::renderContents() {
    int id = 0;

    for (const auto& [sceneID, scene] : this->layer->getScenes()) {
        static uuids::uuid selectedID;

        // Don't list the editor scene
        if (sceneID == this->controls->getEditorScene()->getUUID()) {
            continue;
        }

        ImGui::PushID(++id);
        if (ImGui::Button("X")) {
            this->layer->removeScene(sceneID);
            // todo(editor): only unselect if its currently selected
            this->controls->setSelectedEntity(nullptr);
            this->controls->setSelectedScene(nullptr);
            this->inspector->setSelected(nullptr);
            selectedID = {};
            ImGui::PopID();
            break;
        }
        ImGui::SameLine();
        if (ImGui::Button("S")) {
            this->controls->setSelectedEntity(nullptr);
            this->controls->setSelectedScene(scene.get());
            this->inspector->setSelected(nullptr);
            selectedID = sceneID;
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader(scene->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (const auto& [entityID, entity] : scene->getEntities()) {
                ImGui::PushID(++id);
                if (ImGui::Button("X")) {
                    scene->removeEntity(entityID);
                    // todo(editor): only unselect if its currently selected
                    this->controls->setSelectedEntity(nullptr);
                    this->controls->setSelectedScene(nullptr);
                    this->inspector->setSelected(nullptr);
                    selectedID = {};
                    ImGui::PopID();
                    break;
                }
                ImGui::SameLine();
                if (ImGui::Selectable(entity->getName().c_str(), entityID == selectedID)) {
                    this->controls->setSelectedEntity(entity.get());
                    this->controls->setSelectedScene(nullptr);
                    this->inspector->setSelected(entity.get());
                    selectedID = entityID;
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
