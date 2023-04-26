#include "InspectorPanel.h"

#include <magic_enum.hpp>
#include <entity/component/AngelScriptComponent.h>
#include <entity/component/AudioNoiseComponent.h>
#include <entity/component/AudioSfxrComponent.h>
#include <entity/component/AudioSpeechComponent.h>
#include <entity/component/AudioWavComponent.h>
#include <entity/component/AudioWavStreamComponent.h>
#include <entity/component/CameraComponent.h>
#include <entity/component/LightComponents.h>
#include <entity/component/MeshComponent.h>
#include <entity/component/MeshDynamicComponent.h>
#include <entity/component/NameComponent.h>
#include <entity/component/UUIDComponent.h>
#include <resource/provider/FilesystemResourceProvider.h>

using namespace chira;

InspectorPanel::InspectorPanel()
	    : IPanel("Inspector", true)
        , curEnt(nullptr) {}

void InspectorPanel::setEntity(Entity* newEnt) {
	this->curEnt = newEnt;
}

Entity* InspectorPanel::getEntity() const {
	return this->curEnt;
}

void InspectorPanel::renderContents() {
    if (!this->curEnt)
        return;

    static ImGui::FileBrowser filePicker{ImGuiFileBrowserFlags_CloseOnEsc};

    char nameBuf[512] {0};
    if (!this->curEnt->getName().empty()) {
        memcpy(nameBuf, this->curEnt->getName().c_str(), this->curEnt->getName().size() > 511 ? 511 : this->curEnt->getName().size());
    }

    ImGui::InputText("##Name", nameBuf, sizeof(nameBuf), ImGuiInputTextFlags_EnterReturnsTrue);

    if (nameBuf[0] && strcmp(nameBuf, this->curEnt->getName().c_str()) != 0) {
        if (auto nameComponent = this->curEnt->tryGetComponent<NameComponent>()) {
            nameComponent->name = nameBuf;
        } else {
            this->curEnt->addComponent<NameComponent>(nameBuf);
        }
    }

    // we always want to show the uuid since 2+ entities can share a name
    ImGui::TextDisabled("%s", uuids::to_string(this->curEnt->getUUID()).c_str());

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
            "Camera",
            "Directional Light",
            "Point Light",
            "Spot Light",
            "Mesh",
            "Mesh Dynamic"
        };
        for (const auto& component : components) {
            if (ImGui::Selectable(component.c_str())) {
                if (component == "AngelScript") {
                    this->curEnt->tryRemoveComponent<AngelScriptComponent>();
                    this->curEnt->addComponent<AngelScriptComponent>();
                } else if (component == "Audio Noise") {
                    this->curEnt->tryRemoveComponent<AudioNoiseComponent>();
                    this->curEnt->addComponent<AudioNoiseComponent>(SoLoud::Noise::NOISETYPES::WHITE);
                } else if (component == "Audio Sfxr") {
                    this->curEnt->tryRemoveComponent<AudioSfxrComponent>();
                    this->curEnt->addComponent<AudioSfxrComponent>(SoLoud::Sfxr::SFXR_PRESETS::COIN);
                } else if (component == "Audio Speech") {
                    this->curEnt->tryRemoveComponent<AudioSpeechComponent>();
                    this->curEnt->addComponent<AudioSpeechComponent>();
                } else if (component == "Audio Wav") {
                    this->curEnt->tryRemoveComponent<AudioWavComponent>();
                    this->curEnt->addComponent<AudioWavComponent>();
                } else if (component == "Audio Wav Stream") {
                    this->curEnt->tryRemoveComponent<AudioWavStreamComponent>();
                    this->curEnt->addComponent<AudioWavStreamComponent>();
                } else if (component == "Camera") {
                    this->curEnt->tryRemoveComponent<CameraComponent>();
                    this->curEnt->addComponent<CameraComponent>();
                } else if (component == "Directional Light") {
                    this->curEnt->tryRemoveComponent<DirectionalLightComponent>();
                    this->curEnt->addComponent<DirectionalLightComponent>();
                } else if (component == "Point Light") {
                    this->curEnt->tryRemoveComponent<PointLightComponent>();
                    this->curEnt->addComponent<PointLightComponent>();
                } else if (component == "Spot Light") {
                    this->curEnt->tryRemoveComponent<SpotLightComponent>();
                    this->curEnt->addComponent<SpotLightComponent>();
                } else if (component == "Mesh") {
                    this->curEnt->tryRemoveComponent<MeshComponent>();
                    this->curEnt->addComponent<MeshComponent>();
                } else if (component == "Mesh Dynamic") {
                    this->curEnt->tryRemoveComponent<MeshDynamicComponent>();
                    this->curEnt->addComponent<MeshDynamicComponent>();
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
        auto pos = this->curEnt->getTransform().getPosition();
        auto rot = this->curEnt->getTransform().getRotationEuler();
        auto scl = this->curEnt->getTransform().getScale();

		ImGui::DragFloat3("Position", &pos.x, 0.05f);
        ImGui::DragFloat3("Rotation", &rot.x, 0.05f);
        ImGui::DragFloat3("Scale",    &scl.x, 0.05f);

        this->curEnt->getTransform().setPosition(pos);
        this->curEnt->getTransform().setRotation(rot);
        this->curEnt->getTransform().setScale(scl);
	}

    if (auto component = this->curEnt->tryGetComponent<AngelScriptComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<AngelScriptComponent>();
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
                this->curEnt->tryRemoveComponent<AngelScriptComponent>();
                this->curEnt->addComponent<AngelScriptComponent>(path);
            }
            filePicker.ClearSelected();
        }
    }
    if ([[maybe_unused]] auto component = this->curEnt->tryGetComponent<AudioNoiseComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<AudioNoiseComponent>();
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Noise", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
    if ([[maybe_unused]] auto component = this->curEnt->tryGetComponent<AudioSfxrComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<AudioSfxrComponent>();
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Sfxr", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
    if ([[maybe_unused]] auto component = this->curEnt->tryGetComponent<AudioSpeechComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<AudioSpeechComponent>();
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Speech", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
    if ([[maybe_unused]] auto component = this->curEnt->tryGetComponent<AudioWavComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<AudioWavComponent>();
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Wav", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
    if ([[maybe_unused]] auto component = this->curEnt->tryGetComponent<AudioWavStreamComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<AudioWavStreamComponent>();
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Wav Stream", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
    if (auto component = this->curEnt->tryGetComponent<CameraComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<CameraComponent>();
        }
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
            ImGui::DragFloat("FOV", &component->fov, 0.05f, 0.f, M_PI * 2.f);
            ImGui::DragFloat("Near Distance", &component->nearDistance, 0.05f, 0.001f);
            ImGui::DragFloat("Far Distance", &component->farDistance, 0.05f, 0.001f);
            ImGui::DragFloat("Ortho Size", &component->orthoSize, 0.05f, 0.001f);
            ImGui::Checkbox("Active", &component->active);
        }
    }
    if (auto component = this->curEnt->tryGetComponent<DirectionalLightComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<DirectionalLightComponent>();
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Ambient", &component->ambient.x);
            ImGui::ColorEdit3("Diffuse", &component->diffuse.x);
            ImGui::ColorEdit3("Specular", &component->specular.x);
        }
    }
    if (auto component = this->curEnt->tryGetComponent<PointLightComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<PointLightComponent>();
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Ambient", &component->ambient.x);
            ImGui::ColorEdit3("Diffuse", &component->diffuse.x);
            ImGui::ColorEdit3("Specular", &component->specular.x);
            ImGui::DragFloat3("Falloff", &component->falloff.x);
        }
    }
    if (auto component = this->curEnt->tryGetComponent<SpotLightComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<SpotLightComponent>();
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Diffuse", &component->diffuse.x);
            ImGui::ColorEdit3("Specular", &component->specular.x);
            ImGui::DragFloat3("Falloff", &component->falloff.x);
            ImGui::DragFloat2("Cutoff", &component->cutoff.x);
        }
    }
    if (auto component = this->curEnt->tryGetComponent<MeshComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<MeshComponent>();
        }
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
                this->curEnt->tryRemoveComponent<MeshComponent>();
                this->curEnt->addComponent<MeshComponent>(path);
            }
            filePicker.ClearSelected();
        }
    }
    if ([[maybe_unused]] auto component = this->curEnt->tryGetComponent<MeshDynamicComponent>()) {
        if (ImGui::Button("X")) {
            this->curEnt->tryRemoveComponent<MeshDynamicComponent>();
        }
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Mesh Dynamic", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("todo...");
        }
    }
}
