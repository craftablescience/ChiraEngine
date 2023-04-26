#include "InspectorPanel.h"

#include <algorithm>
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

    char nameBuf[512] {0};
    if (!this->curEnt->getName().empty()) {
        memcpy(nameBuf, this->curEnt->getName().c_str(), std::clamp(this->curEnt->getName().size(), 0ull, 512ull - 1ull));
    }

    ImGui::InputText("##Name", nameBuf, sizeof(nameBuf), ImGuiInputTextFlags_EnterReturnsTrue);

    if (nameBuf[0] && strcmp(nameBuf, this->curEnt->getName().c_str()) != 0) {
        if (auto nameComponent = this->curEnt->tryGetComponent<NameComponent>()) {
            nameComponent->name = nameBuf;
        } else {
            this->curEnt->addComponent<NameComponent>(nameBuf);
        }
    }

    // we always want to show the uuid since 2+ entities can share a name.
    ImGui::TextDisabled("%s", uuids::to_string(this->curEnt->getUUID()).c_str());

	ImGui::Separator();

    if (ImGui::BeginPopupContextItem("Add Component")) {
        // todo: replace this predefined list with a list of registered components
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
            "Mesh Dynamic",
            "Name",
            "Skybox",
            "Transform"
        };
        for (const auto& component : components) {
            ImGui::Selectable(component.c_str());
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

    if (auto scriptComp = this->curEnt->tryGetComponent<AngelScriptComponent>()) {
        static ImGui::FileBrowser scriptDialog{ImGuiFileBrowserFlags_CloseOnEsc};

        if (ImGui::CollapsingHeader("Script")) {
            if (ImGui::Button("Pick Script")) {
                scriptDialog.Open();
            }

            ImGui::Text("%s", scriptComp->getScript()->getIdentifier().data());
        }

        // Model Dialog specific logic
        scriptDialog.Display();
        if (scriptDialog.HasSelected()) {
            std::string path = FilesystemResourceProvider::getResourceIdentifier(scriptDialog.GetSelected().string());
            if (!path.empty()) {
                this->curEnt->tryRemoveComponent<AngelScriptComponent>();
                this->curEnt->addComponent<AngelScriptComponent>(path);
            }

            scriptDialog.ClearSelected();
        }
    }
}
