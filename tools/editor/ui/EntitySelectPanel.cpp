#include "EntitySelectPanel.h"

#include <entity/Viewport.h>
#include "ControlsPanel.h"
#include "InspectorPanel.h"

using namespace chira;

EntitySelectPanel::EntitySelectPanel(Viewport* layer_, ControlsPanel* controls_, InspectorPanel* inspector_)
        : IPanel("Entity Select", true)
        , viewport(layer_)
        , controls(controls_)
        , inspector(inspector_) {}

void EntitySelectPanel::renderContents() {
    int id = 0;

    for (const auto& [sceneID, scene] : this->viewport->getScenes()) {
        static uuids::uuid selectedID;

#ifndef DEBUG
        // Don't list the editor scene
        if (sceneID == this->controls->getEditorScene()->getUUID()) {
            continue;
        }
#endif

        ImGui::PushID(++id);
        if (ImGui::Button("X")) {
            this->viewport->removeScene(sceneID);
            // todo(editor): only unselect if its currently selected
            this->controls->setSelectedEntity(nullptr);
            this->controls->setSelectedScene(nullptr);
            this->inspector->setSelectedEntity(nullptr);
            this->inspector->setSelectedScene(nullptr);
            selectedID = {};
            ImGui::PopID();
            break;
        }

        ImGui::SameLine();

        if (ImGui::Button("$")) {
            this->controls->setSelectedEntity(nullptr);
            this->controls->setSelectedScene(scene.get());
            this->inspector->setSelectedEntity(nullptr);
            this->inspector->setSelectedScene(scene.get());
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
                    this->inspector->setSelectedEntity(nullptr);
                    this->inspector->setSelectedScene(nullptr);
                    selectedID = {};
                    ImGui::PopID();
                    break;
                }

                ImGui::SameLine();

                if (entity->hasComponent<NoRenderTagComponent>() && ImGui::Button("S")) {
                    entity->removeComponent<NoRenderTagComponent>();
                } else if (!entity->hasComponent<NoRenderTagComponent>() && ImGui::Button("H")) {
                    entity->addTagComponent<NoRenderTagComponent>();
                }

                ImGui::SameLine();

                if (ImGui::Selectable(entity->getName().c_str(), entityID == selectedID)) {
                    this->controls->setSelectedEntity(entity.get());
                    this->controls->setSelectedScene(nullptr);
                    this->inspector->setSelectedEntity(entity.get());
                    this->inspector->setSelectedScene(nullptr);
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
        this->viewport->addScene(sceneName);
    }
}

void EntitySelectPanel::setViewport(Viewport* viewport_) {
    this->viewport = viewport_;
}

Viewport* EntitySelectPanel::getViewport() const {
    return this->viewport;
}
