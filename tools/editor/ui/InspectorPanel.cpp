#include "InspectorPanel.h"

#include <fmt/core.h>
#include <imfilebrowser.h>
#include <magic_enum.hpp>

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
#include <entity/component/MeshSpriteComponent.h>
#include <entity/component/NameComponent.h>
#include <entity/component/SkyboxComponent.h>
#include <entity/component/UUIDComponent.h>
#include <entity/component/LayerComponents.h>

using namespace chira;

CHIRA_GET_MODULE(Audio);

#define REMOVE_BUTTON(component)                                                                                    \
    do {                                                                                                            \
        if (ImGui::Button("X##" #component)) {                                                                      \
            ImGui::OpenPopup("Remove Component?##" #component);                                                     \
        }                                                                                                           \
        if (ImGui::BeginPopupModal("Remove Component?##" #component, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) { \
            ImGui::Text("This component will be removed.\nThis operation cannot be undone!");                       \
            ImGui::Separator();                                                                                     \
            if (ImGui::Button("OK##" #component, ImVec2(120, 0))) {                                                 \
                this->selectedEntity->tryRemoveComponent<component>();                                              \
                ImGui::CloseCurrentPopup();                                                                         \
                ImGui::EndPopup();                                                                                  \
                return;                                                                                             \
            }                                                                                                       \
            ImGui::SetItemDefaultFocus();                                                                           \
            ImGui::SameLine();                                                                                      \
            if (ImGui::Button("Cancel##" #component, ImVec2(120, 0)))                                               \
                ImGui::CloseCurrentPopup();                                                                         \
            ImGui::EndPopup();                                                                                      \
        }                                                                                                           \
    } while (0)

InspectorPanel::InspectorPanel()
	    : IPanel("Inspector", true)
        , selectedEntity(nullptr)
        , selectedScene(nullptr) {}

void InspectorPanel::setSelectedEntity(Entity* selected) {
	this->selectedEntity = selected;
}

void InspectorPanel::setSelectedScene(Scene* selected) {
    this->selectedScene = selected;
}

void InspectorPanel::renderContents() {
    if (this->selectedEntity)
        this->renderContentsForSelectedEntity();
    else if (this->selectedScene)
        this->renderContentsForSelectedScene();
}

void InspectorPanel::renderContentsForSelectedEntity() {
    static ImGui::FileBrowser filePicker{ImGuiFileBrowserFlags_CloseOnEsc};

    char nameBuf[512] {0};
    if (!this->selectedEntity->getName().empty()) {
        memcpy(nameBuf, this->selectedEntity->getName().c_str(), this->selectedEntity->getName().size() > 511 ? 511 : this->selectedEntity->getName().size());
    }

    ImGui::InputText("##Name", nameBuf, sizeof(nameBuf), ImGuiInputTextFlags_EnterReturnsTrue);

    int currentLayer;
    enumerate(LAYER_COMPONENTS, [this, &currentLayer](std::size_t index, auto layer) {
        if (this->selectedEntity->hasComponent<decltype(layer)>()) {
            currentLayer = static_cast<int>(index);
        }
    });

    if (ImGui::BeginCombo("Object Layer", fmt::format("Layer {}", currentLayer).c_str())) {
        enumerate(LAYER_COMPONENTS, [&](std::size_t index, auto layer) {
            if (ImGui::Selectable(fmt::format("Layer {}", index).c_str())) {
                foreach(LAYER_COMPONENTS, [&](auto layer) {
                    this->selectedEntity->template tryRemoveComponent<decltype(layer)>();
                });
                this->selectedEntity->template addTagComponent<decltype(layer)>();
            }
        });
        ImGui::EndCombo();
    }

    if (nameBuf[0] && strcmp(nameBuf, this->selectedEntity->getName().c_str()) != 0) {
        if (auto nameComponent = this->selectedEntity->tryGetComponent<NameComponent>()) {
            nameComponent->name = nameBuf;
        } else {
            this->selectedEntity->addComponent<NameComponent>(nameBuf);
        }
    }

    // we always want to show the uuid since 2+ entities can share a name
    ImGui::TextDisabled("%s", uuids::to_string(this->selectedEntity->getUUID()).c_str());

	ImGui::Separator();

    if (ImGui::BeginPopupContextItem("Add Component")) {
        // todo(editor): replace this predefined list with a component registry
        // todo(editor): don't show components that are already on the entity

        // Name, UUID, Transform are excluded because they are already viewable/editable above this
        // Skybox can only be added to scene entities (literally entities that are scenes)
        // Also don't include tags, there should be a dedicated interface for tags
        std::string components[] = {
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
            "Mesh Sprite",
        };
        for (const auto& component : components) {
            if (ImGui::Selectable(component.c_str())) {
                if (component == "Audio Noise") {
                    this->selectedEntity->tryRemoveComponent<AudioNoiseComponent>();
                    this->selectedEntity->addComponent<AudioNoiseComponent>();
                } else if (component == "Audio Sfxr") {
                    this->selectedEntity->tryRemoveComponent<AudioSfxrComponent>();
                    this->selectedEntity->addComponent<AudioSfxrComponent>();
                } else if (component == "Audio Speech") {
                    this->selectedEntity->tryRemoveComponent<AudioSpeechComponent>();
                    this->selectedEntity->addComponent<AudioSpeechComponent>();
                } else if (component == "Audio Wav") {
                    this->selectedEntity->tryRemoveComponent<AudioWavComponent>();
                    this->selectedEntity->addComponent<AudioWavComponent>();
                } else if (component == "Audio Wav Stream") {
                    this->selectedEntity->tryRemoveComponent<AudioWavStreamComponent>();
                    this->selectedEntity->addComponent<AudioWavStreamComponent>();
                } else if (component == "Billboard") {
                    this->selectedEntity->tryRemoveComponent<BillboardComponent>();
                    this->selectedEntity->addComponent<BillboardComponent>();
                } else if (component == "Camera") {
                    this->selectedEntity->tryRemoveComponent<CameraComponent>();
                    this->selectedEntity->addComponent<CameraComponent>();
                } else if (component == "Directional Light") {
                    this->selectedEntity->tryRemoveComponent<DirectionalLightComponent>();
                    this->selectedEntity->addComponent<DirectionalLightComponent>();
                } else if (component == "Point Light") {
                    this->selectedEntity->tryRemoveComponent<PointLightComponent>();
                    this->selectedEntity->addComponent<PointLightComponent>();
                } else if (component == "Spot Light") {
                    this->selectedEntity->tryRemoveComponent<SpotLightComponent>();
                    this->selectedEntity->addComponent<SpotLightComponent>();
                } else if (component == "Mesh") {
                    this->selectedEntity->tryRemoveComponent<MeshComponent>();
                    this->selectedEntity->addComponent<MeshComponent>();
                } else if (component == "Mesh Dynamic") {
                    this->selectedEntity->tryRemoveComponent<MeshDynamicComponent>();
                    this->selectedEntity->addComponent<MeshDynamicComponent>();
                } else if (component == "Mesh Sprite") {
                    this->selectedEntity->tryRemoveComponent<MeshSpriteComponent>();
                    this->selectedEntity->addComponent<MeshSpriteComponent>();
                }
            }
        }
        ImGui::EndPopup();
    }

    if (ImGui::Button("Add Component...")) {
        ImGui::OpenPopup("Add Component");
    }

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto pos = this->selectedEntity->getTransform().getPosition();
        auto rot = this->selectedEntity->getTransform().getRotationEuler();
        auto scl = this->selectedEntity->getTransform().getScale();

		ImGui::DragFloat3("Position", &pos.x, 0.05f);
        ImGui::DragFloat3("Rotation", &rot.x, 0.05f);
        ImGui::DragFloat3("Scale",    &scl.x, 0.05f);

        this->selectedEntity->getTransform().setPosition(pos);
        this->selectedEntity->getTransform().setRotation(rot);
        this->selectedEntity->getTransform().setScale(scl);
	}

#if 0
    if (auto component = this->selectedEntity->tryGetComponent<AngelScriptComponent>()) {
        REMOVE_BUTTON(AngelScriptComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("AngelScript", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("Pick Script")) {
                filePicker.Open();
            }

            ImGui::Text("%s", component->getScript()->getPath().data());
        }

        filePicker.Display();
        if (filePicker.HasSelected()) {
            std::string path = filePicker.GetSelected().string();
            if (!path.empty()) {
                this->selectedEntity->tryRemoveComponent<AngelScriptComponent>();
                this->selectedEntity->addComponent<AngelScriptComponent>(path);
            }
            filePicker.ClearSelected();
        }
    }
#endif
    if (auto component = this->selectedEntity->tryGetComponent<AudioNoiseComponent>()) {
        REMOVE_BUTTON(AudioNoiseComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Noise", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginCombo("Noise Type", component->getNoiseType() >= 0 ? magic_enum::enum_name(component->getNoiseType()).data() : "CUSTOM")) {
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Noise::NOISETYPES::WHITE).data())) {
                    component->setNoiseType(SoLoud::Noise::NOISETYPES::WHITE);
                }
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Noise::NOISETYPES::PINK).data())) {
                    component->setNoiseType(SoLoud::Noise::NOISETYPES::PINK);
                }
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Noise::NOISETYPES::BLUEISH).data())) {
                    component->setNoiseType(SoLoud::Noise::NOISETYPES::BLUEISH);
                }
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Noise::NOISETYPES::BROWNISH).data())) {
                    component->setNoiseType(SoLoud::Noise::NOISETYPES::BROWNISH);
                }
                if (ImGui::Selectable("CUSTOM")) {
                    component->setNoiseType(static_cast<SoLoud::Noise::NOISETYPES>(-1));
                }
                ImGui::EndCombo();
            }
            if (component->getNoiseType() < 0) {
                ImGui::DragFloat("Octave 0", &component->noise.mOctaveScale[0]);
                ImGui::DragFloat("Octave 1", &component->noise.mOctaveScale[1]);
                ImGui::DragFloat("Octave 2", &component->noise.mOctaveScale[2]);
                ImGui::DragFloat("Octave 3", &component->noise.mOctaveScale[3]);
                ImGui::DragFloat("Octave 4", &component->noise.mOctaveScale[4]);
                ImGui::DragFloat("Octave 5", &component->noise.mOctaveScale[5]);
                ImGui::DragFloat("Octave 6", &component->noise.mOctaveScale[6]);
                ImGui::DragFloat("Octave 7", &component->noise.mOctaveScale[7]);
                ImGui::DragFloat("Octave 8", &component->noise.mOctaveScale[8]);
                ImGui::DragFloat("Octave 9", &component->noise.mOctaveScale[9]);
            }
            if (ImGui::Button("Preview Audio")) {
                auto handle = g_Audio->get().play(component->noise);
                g_Audio->get().scheduleStop(handle, 2.0);
            }
        }
    }
    if ([[maybe_unused]] auto component = this->selectedEntity->tryGetComponent<AudioSfxrComponent>()) {
        REMOVE_BUTTON(AudioSfxrComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Sfxr", ImGuiTreeNodeFlags_DefaultOpen)) {
            int seed = component->getSfxrSeed();
            if (ImGui::BeginCombo("Preset", component->getSfxrPreset() >= 0 ? magic_enum::enum_name(component->getSfxrPreset()).data() : "CUSTOM")) {
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Sfxr::SFXR_PRESETS::COIN).data())) {
                    component->setSfxrFromPreset(SoLoud::Sfxr::SFXR_PRESETS::COIN);
                }
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Sfxr::SFXR_PRESETS::LASER).data())) {
                    component->setSfxrFromPreset(SoLoud::Sfxr::SFXR_PRESETS::LASER);
                }
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Sfxr::SFXR_PRESETS::EXPLOSION).data())) {
                    component->setSfxrFromPreset(SoLoud::Sfxr::SFXR_PRESETS::EXPLOSION);
                }
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Sfxr::SFXR_PRESETS::POWERUP).data())) {
                    component->setSfxrFromPreset(SoLoud::Sfxr::SFXR_PRESETS::POWERUP);
                }
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Sfxr::SFXR_PRESETS::HURT).data())) {
                    component->setSfxrFromPreset(SoLoud::Sfxr::SFXR_PRESETS::HURT);
                }
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Sfxr::SFXR_PRESETS::JUMP).data())) {
                    component->setSfxrFromPreset(SoLoud::Sfxr::SFXR_PRESETS::JUMP);
                }
                if (ImGui::Selectable(magic_enum::enum_name(SoLoud::Sfxr::SFXR_PRESETS::BLIP).data())) {
                    component->setSfxrFromPreset(SoLoud::Sfxr::SFXR_PRESETS::BLIP);
                }
                if (ImGui::Selectable("CUSTOM")) {
                    component->setSfxrFromPreset(static_cast<SoLoud::Sfxr::SFXR_PRESETS>(-1), seed);
                }
                ImGui::EndCombo();
            }
            if (component->getSfxrPreset() >= 0) {
                ImGui::InputInt("Seed", &seed);
                component->setSfxrFromPreset(component->getSfxrPreset(), seed);
            } else {
                ImGui::Text("%s", !component->getSfxrConfigID().empty() ? component->getSfxrConfigID().data() : "No config file selected.");
                if (ImGui::Button("Pick Audio")) {
                    filePicker.Open();
                }
            }
            if (ImGui::Button("Preview Audio")) {
                g_Audio->get().play(component->sfxr);
            }
        }

        filePicker.Display();
        if (filePicker.HasSelected()) {
            std::string path = filePicker.GetSelected().string();
            if (!path.empty()) {
                this->selectedEntity->tryRemoveComponent<AudioSfxrComponent>();
                this->selectedEntity->addComponent<AudioSfxrComponent>(path);
            }
            filePicker.ClearSelected();
        }
    }
    if (auto component = this->selectedEntity->tryGetComponent<AudioSpeechComponent>()) {
        REMOVE_BUTTON(AudioSpeechComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Speech", ImGuiTreeNodeFlags_DefaultOpen)) {
            char buf[2048] {0};
            if (!component->getSpeechText().empty()) {
                memcpy(buf, component->getSpeechText().data(), component->getSpeechText().size() > 2047 ? 2047 : component->getSpeechText().size());
            }
            ImGui::InputTextMultiline("Voice Line", buf, sizeof(buf) - 1);
            component->setSpeechText(buf);

            if (ImGui::Button("Preview Audio")) {
                g_Audio->get().play(component->speech);
            }
        }
    }
    if (auto component = this->selectedEntity->tryGetComponent<AudioWavComponent>()) {
        REMOVE_BUTTON(AudioWavComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Wav", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("%s", component->wavFile->getPath().data());

            if (ImGui::Button("Pick Audio")) {
                filePicker.Open();
            }

            if (ImGui::Button("Preview Audio")) {
                g_Audio->get().play(component->wav);
            }
        }

        filePicker.Display();
        if (filePicker.HasSelected()) {
            std::string path = filePicker.GetSelected().string();
            if (!path.empty()) {
                this->selectedEntity->tryRemoveComponent<AudioWavComponent>();
                this->selectedEntity->addComponent<AudioWavComponent>(path);
            }
            filePicker.ClearSelected();
        }
    }
    if (auto component = this->selectedEntity->tryGetComponent<AudioWavStreamComponent>()) {
        REMOVE_BUTTON(AudioWavStreamComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Audio Wav Stream", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("%s", component->wavFile->getPath().data());

            if (ImGui::Button("Pick Audio")) {
                filePicker.Open();
            }

            if (ImGui::Button("Preview Audio")) {
                g_Audio->get().play(component->wavStream);
            }
        }

        filePicker.Display();
        if (filePicker.HasSelected()) {
            std::string path = filePicker.GetSelected().string();
            if (!path.empty()) {
                this->selectedEntity->tryRemoveComponent<AudioWavStreamComponent>();
                this->selectedEntity->addComponent<AudioWavStreamComponent>(path);
            }
            filePicker.ClearSelected();
        }
    }
    if (auto component = this->selectedEntity->tryGetComponent<BillboardComponent>()) {
        REMOVE_BUTTON(BillboardComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Billboard", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Checkbox("X Axis", &component->x);
            ImGui::Checkbox("Y Axis", &component->y);
            ImGui::Checkbox("Z Axis", &component->z);
        }
    }
    if (auto component = this->selectedEntity->tryGetComponent<CameraComponent>()) {
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

            if (ImGui::BeginCombo("##active_layers_combo", "Active Layers")) {
                for (int i = 0; i < MAX_LAYER_COMPONENTS; i++) {
                    bool valueActive = static_cast<bool>((component->activeLayers) & (1 << i));
                    bool newValue = valueActive;
                    ImGui::Checkbox(fmt::format("Layer {}", i).c_str(), &newValue);

                    if (valueActive != newValue) {
                        if (newValue) {
                            component->activeLayers = component->activeLayers | (1 << i);
                        } else {
                            component->activeLayers = component->activeLayers & ~(1 << i);
                        }
                    }
                }
                ImGui::EndCombo();
            }
        }
    }
    if (auto component = this->selectedEntity->tryGetComponent<DirectionalLightComponent>()) {
        REMOVE_BUTTON(DirectionalLightComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Ambient", &component->ambient.x);
            ImGui::ColorEdit3("Diffuse", &component->diffuse.x);
            ImGui::ColorEdit3("Specular", &component->specular.x);
        }
    }
    if (auto component = this->selectedEntity->tryGetComponent<PointLightComponent>()) {
        REMOVE_BUTTON(PointLightComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Ambient", &component->ambient.x);
            ImGui::ColorEdit3("Diffuse", &component->diffuse.x);
            ImGui::ColorEdit3("Specular", &component->specular.x);
            ImGui::DragFloat3("Falloff", &component->falloff.x);
        }
    }
    if (auto component = this->selectedEntity->tryGetComponent<SpotLightComponent>()) {
        REMOVE_BUTTON(SpotLightComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::ColorEdit3("Diffuse", &component->diffuse.x);
            ImGui::ColorEdit3("Specular", &component->specular.x);
            ImGui::DragFloat3("Falloff", &component->falloff.x);
            ImGui::DragFloat2("Cutoff", &component->cutoff.x);
        }
    }
    if (auto component = this->selectedEntity->tryGetComponent<MeshComponent>()) {
        REMOVE_BUTTON(MeshComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("%s", component->mesh->getPath().data());

            if (ImGui::Button("Pick Mesh")) {
                filePicker.Open();
            }
        }

        filePicker.Display();
        if (filePicker.HasSelected()) {
            std::string path = filePicker.GetSelected().string();
            if (!path.empty()) {
                this->selectedEntity->tryRemoveComponent<MeshComponent>();
                this->selectedEntity->addComponent<MeshComponent>(path);
            }
            filePicker.ClearSelected();
        }
    }
    if ([[maybe_unused]] auto component = this->selectedEntity->tryGetComponent<MeshDynamicComponent>()) {
        REMOVE_BUTTON(MeshDynamicComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Mesh Dynamic", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("This component is only editable in code.");
        }
    }
    if (auto component = this->selectedEntity->tryGetComponent<MeshSpriteComponent>()) {
        REMOVE_BUTTON(MeshSpriteComponent);
        ImGui::SameLine();
        if (ImGui::CollapsingHeader("Mesh Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("%s", component->sprite.getMaterial()->getPath().data());

            if (ImGui::Button("Pick Material")) {
                filePicker.Open();
            }
        }

        filePicker.Display();
        if (filePicker.HasSelected()) {
            std::string path = filePicker.GetSelected().string();
            if (!path.empty()) {
                const auto size = component->size;
                this->selectedEntity->tryRemoveComponent<MeshSpriteComponent>();
                this->selectedEntity->addComponent<MeshSpriteComponent>(size, path);
            }
            filePicker.ClearSelected();
        }
    }
}

void InspectorPanel::renderContentsForSelectedScene() {
    static ImGui::FileBrowser filePicker{ImGuiFileBrowserFlags_CloseOnEsc};

    if (ImGui::BeginPopupContextItem("Add Component")) {
        // Scene-specific components
        std::string components[] = {
                "Skybox",
        };
        for (const auto &component: components) {
            if (ImGui::Selectable(component.c_str())) {
                if (component == "Skybox") {
                    this->selectedScene->tryRemoveComponent<SkyboxComponent>();
                    this->selectedScene->addComponent<SkyboxComponent>();
                }
            }
        }
        ImGui::EndPopup();
    }

    if (ImGui::Button("Add Component...")) {
        ImGui::OpenPopup("Add Component");
    }

    if (auto component = this->selectedScene->tryGetComponent<SkyboxComponent>()) {
        if (ImGui::Button("X")) {
            ImGui::OpenPopup("Remove Component?##" "SkyboxComponent");
        }
        if (ImGui::BeginPopupModal("Remove Component?##" "SkyboxComponent", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("This component will be removed.\nThis operation cannot be undone!");
            ImGui::Separator();
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                this->selectedScene->tryRemoveComponent<SkyboxComponent>();
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                return;
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::SameLine();

        if (ImGui::CollapsingHeader("Skybox", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("%s", component->skybox.getMaterial()->getPath().data());

            if (ImGui::Button("Pick Skybox")) {
                filePicker.Open();
            }
        }

        filePicker.Display();
        if (filePicker.HasSelected()) {
            std::string path = filePicker.GetSelected().string();
            if (!path.empty()) {
                this->selectedScene->tryRemoveComponent<SkyboxComponent>();
                this->selectedScene->addComponent<SkyboxComponent>(path);
            }
            filePicker.ClearSelected();
        }
    }
}
