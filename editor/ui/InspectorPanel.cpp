#include "InspectorPanel.h"

#include <magic_enum.hpp>
#include <entity/component/AngelScriptComponent.h>
#include <entity/component/AudioNoiseComponent.h>
#include <entity/component/AudioSfxrComponent.h>
#include <entity/component/AudioSpeechComponent.h>
#include <entity/component/AudioWavComponent.h>
#include <entity/component/AudioWavStreamComponent.h>
#include <entity/component/BillboardComponent.h>
#include <entity/component/CameraComponent.h>
#include <entity/component/LightComponents.h>
#include <entity/component/MeshComponent.h>
#include <entity/component/MeshDynamicComponent.h>
#include <entity/component/NameComponent.h>
#include <entity/component/UUIDComponent.h>
#include <resource/provider/FilesystemResourceProvider.h>

using namespace chira;

#define REMOVE_BUTTON(comptype) if (ImGui::Button("X")) {\
    ImGui::OpenPopup("Remove Component?##" #comptype);\
} \
if (ImGui::BeginPopupModal("Remove Component?##" #comptype, NULL, ImGuiWindowFlags_AlwaysAutoResize))\
{\
    ImGui::Text("This component will be removed.\nThis operation cannot be undone!");\
    ImGui::Separator();\
\
    if (ImGui::Button("OK", ImVec2(120, 0))) { \
        this->selected->tryRemoveComponent<##comptype>();\
        ImGui::CloseCurrentPopup();\
    } \
    ImGui::SetItemDefaultFocus();\
    ImGui::SameLine();\
    if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }\
    ImGui::EndPopup();\
}\

InspectorPanel::InspectorPanel()
	    : IPanel("Inspector", true)
        , selected(nullptr) {}

void InspectorPanel::setSelected(Entity* selected_) {
	this->selected = selected_;
}

void InspectorPanel::renderContents() {
    if (!this->selected)
        return;

    static ImGui::FileBrowser filePicker{ImGuiFileBrowserFlags_CloseOnEsc};

    char nameBuf[512] {0};
    if (!this->selected->getName().empty()) {
        memcpy(nameBuf, this->selected->getName().c_str(), this->selected->getName().size() > 511 ? 511 : this->selected->getName().size());
    }

    ImGui::InputText("##Name", nameBuf, sizeof(nameBuf), ImGuiInputTextFlags_EnterReturnsTrue);

    if (nameBuf[0] && strcmp(nameBuf, this->selected->getName().c_str()) != 0) {
        if (auto nameComponent = this->selected->tryGetComponent<NameComponent>()) {
            nameComponent->name = nameBuf;
        } else {
            this->selected->addComponent<NameComponent>(nameBuf);
        }
    }

    // we always want to show the uuid since 2+ entities can share a name
    ImGui::TextDisabled("%s", uuids::to_string(this->selected->getUUID()).c_str());

	ImGui::Separator();

    if (ImGui::BeginPopupContextItem("Add Component")) {
        // todo(editor): replace this predefined list with a component registry
        // Name, UUID, Transform are excluded because they are already viewable/editable above this
        // Skybox can only be added to scene entities (literally entities that are scenes)
        // Also don't include tags, there should be a dedicated interface for tags
        std::string components[] = {
            "AngelScript",
            "Audio Noise",
            "Audio Sfxr",
            "Audio Speech",
            "Audio Wav",
            "Audio Wav Stream",
            "Billboard",
            "Camera",
            "Directional Light",
            "Point Light",
            "Spot Light",
            "Mesh",
            "Mesh Dynamic",
        };
        for (const auto& component : components) {
            if (ImGui::Selectable(component.c_str())) {
                if (component == "AngelScript") {
                    this->selected->tryRemoveComponent<AngelScriptComponent>();
                    this->selected->addComponent<AngelScriptComponent>();
                } else if (component == "Audio Noise") {
                    this->selected->tryRemoveComponent<AudioNoiseComponent>();
                    this->selected->addComponent<AudioNoiseComponent>();
                } else if (component == "Audio Sfxr") {
                    this->selected->tryRemoveComponent<AudioSfxrComponent>();
                    this->selected->addComponent<AudioSfxrComponent>();
                } else if (component == "Audio Speech") {
                    this->selected->tryRemoveComponent<AudioSpeechComponent>();
                    this->selected->addComponent<AudioSpeechComponent>();
                } else if (component == "Audio Wav") {
                    this->selected->tryRemoveComponent<AudioWavComponent>();
                    this->selected->addComponent<AudioWavComponent>();
                } else if (component == "Audio Wav Stream") {
                    this->selected->tryRemoveComponent<AudioWavStreamComponent>();
                    this->selected->addComponent<AudioWavStreamComponent>();
                } else if (component == "Billboard") {
                    this->selected->tryRemoveComponent<BillboardComponent>();
                    this->selected->addComponent<BillboardComponent>();
                } else if (component == "Camera") {
                    this->selected->tryRemoveComponent<CameraComponent>();
                    this->selected->addComponent<CameraComponent>();
                } else if (component == "Directional Light") {
                    this->selected->tryRemoveComponent<DirectionalLightComponent>();
                    this->selected->addComponent<DirectionalLightComponent>();
                } else if (component == "Point Light") {
                    this->selected->tryRemoveComponent<PointLightComponent>();
                    this->selected->addComponent<PointLightComponent>();
                } else if (component == "Spot Light") {
                    this->selected->tryRemoveComponent<SpotLightComponent>();
                    this->selected->addComponent<SpotLightComponent>();
                } else if (component == "Mesh") {
                    this->selected->tryRemoveComponent<MeshComponent>();
                    this->selected->addComponent<MeshComponent>();
                } else if (component == "Mesh Dynamic") {
                    this->selected->tryRemoveComponent<MeshDynamicComponent>();
                    this->selected->addComponent<MeshDynamicComponent>();
                }
            }
        }
        ImGui::EndPopup();
    }

    if (ImGui::Button("+")) {
        ImGui::OpenPopup("Add Component");
    }

	// testing
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto pos = this->selected->getTransform().getPosition();
        auto rot = this->selected->getTransform().getRotationEuler();
        auto scl = this->selected->getTransform().getScale();

		ImGui::DragFloat3("Position", &pos.x, 0.05f);
        ImGui::DragFloat3("Rotation", &rot.x, 0.05f);
        ImGui::DragFloat3("Scale",    &scl.x, 0.05f);

        this->selected->getTransform().setPosition(pos);
        this->selected->getTransform().setRotation(rot);
        this->selected->getTransform().setScale(scl);
	}

    if (auto component = this->selected->tryGetComponent<AngelScriptComponent>()) {
        REMOVE_BUTTON(AngelScriptComponent);
        if (ImGui::Button("X")) {
            this->selected->tryRemoveComponent<AngelScriptComponent>();
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("AngelScript", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("Pick Script")) {
                filePicker.Open();
            }

            ImGui::Text("%s", component->getScript()->getIdentifier().data());
        }

        filePicker.Display();
        if (filePicker.HasSelected()) {
            std::string path = FilesystemResourceProvider::getResourceIdentifier(filePicker.GetSelected().string());
            if (!path.empty()) {
                this->selected->tryRemoveComponent<AngelScriptComponent>();
                this->selected->addComponent<AngelScriptComponent>(path);
            }
            filePicker.ClearSelected();
        }
    }
    if ([[maybe_unused]] auto component = this->selected->tryGetComponent<AudioNoiseComponent>()) {
        REMOVE_BUTTON(AudioNoiseComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Noise", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
    if ([[maybe_unused]] auto component = this->selected->tryGetComponent<AudioSfxrComponent>()) {
        REMOVE_BUTTON(AudioSfxrComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Sfxr", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
    if ([[maybe_unused]] auto component = this->selected->tryGetComponent<AudioSpeechComponent>()) {
        REMOVE_BUTTON(AudioSpeechComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Speech", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
    if ([[maybe_unused]] auto component = this->selected->tryGetComponent<AudioWavComponent>()) {
        REMOVE_BUTTON(AudioWavComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Wav", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
    if ([[maybe_unused]] auto component = this->selected->tryGetComponent<AudioWavStreamComponent>()) {
        REMOVE_BUTTON(AudioWavStreamComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Wav Stream", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
    if (auto component = this->selected->tryGetComponent<BillboardComponent>()) {
        REMOVE_BUTTON(BillboardComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Billboard", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("X Axis", &component->x);
            ImGui::Checkbox("Y Axis", &component->y);
            ImGui::Checkbox("Z Axis", &component->z);
        }
    }
    if (auto component = this->selected->tryGetComponent<CameraComponent>()) {
        REMOVE_BUTTON(CameraComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginCombo("Projection Mode", magic_enum::enum_name(component->projectionMode).data())) {
                if (ImGui::Selectable(magic_enum::enum_name(CameraComponent::ProjectionMode::PERSPECTIVE).data())) {
                    component->projectionMode = CameraComponent::ProjectionMode::PERSPECTIVE;
                }
                if (ImGui::Selectable(magic_enum::enum_name(CameraComponent::ProjectionMode::ORTHOGRAPHIC).data())) {
                    component->projectionMode = CameraComponent::ProjectionMode::ORTHOGRAPHIC;
                }
                ImGui::EndCombo();
            }
            ImGui::DragFloat("FOV", &component->fov, 0.05f, 0.f, static_cast<float>(M_PI * 2));
            ImGui::DragFloat("Near Distance", &component->nearDistance, 0.05f);
            ImGui::DragFloat("Far Distance", &component->farDistance, 0.05f);
            ImGui::DragFloat("Ortho Size", &component->orthoSize, 0.05f, 0.001f);
            ImGui::Checkbox("Active", &component->active);
        }
    }
    if (auto component = this->selected->tryGetComponent<DirectionalLightComponent>()) {
        REMOVE_BUTTON(DirectionalLightComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Ambient", &component->ambient.x);
            ImGui::ColorEdit3("Diffuse", &component->diffuse.x);
            ImGui::ColorEdit3("Specular", &component->specular.x);
        }
    }
    if (auto component = this->selected->tryGetComponent<PointLightComponent>()) {
        REMOVE_BUTTON(PointLightComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Ambient", &component->ambient.x);
            ImGui::ColorEdit3("Diffuse", &component->diffuse.x);
            ImGui::ColorEdit3("Specular", &component->specular.x);
            ImGui::DragFloat3("Falloff", &component->falloff.x);
        }
    }
    if (auto component = this->selected->tryGetComponent<SpotLightComponent>()) {
        REMOVE_BUTTON(SpotLightComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Diffuse", &component->diffuse.x);
            ImGui::ColorEdit3("Specular", &component->specular.x);
            ImGui::DragFloat3("Falloff", &component->falloff.x);
            ImGui::DragFloat2("Cutoff", &component->cutoff.x);
        }
    }
    if (auto component = this->selected->tryGetComponent<MeshComponent>()) {
        REMOVE_BUTTON(MeshComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("Pick Mesh")) {
                filePicker.Open();
            }

            ImGui::Text("%s", component->getMeshResource()->getIdentifier().data());
        }

        filePicker.Display();
        if (filePicker.HasSelected()) {
            std::string path = FilesystemResourceProvider::getResourceIdentifier(filePicker.GetSelected().string());
            if (!path.empty()) {
                this->selected->tryRemoveComponent<MeshComponent>();
                this->selected->addComponent<MeshComponent>(path);
            }
            filePicker.ClearSelected();
        }
    }
    if ([[maybe_unused]] auto component = this->selected->tryGetComponent<MeshDynamicComponent>()) {
        REMOVE_BUTTON(MeshDynamicComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Mesh Dynamic", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
}
