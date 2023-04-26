#include "InspectorPanel.h"

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

    // I wish imgui used std::string holy shit
    char buf[2048];

    if (sizeof(this->curEnt->getName().c_str()) < sizeof(buf))
        strncpy(buf, this->curEnt->getName().c_str(), sizeof(buf));

    ImGui::InputText("##Name", buf, sizeof(buf));

    if (strcmp(buf, this->curEnt->getName().c_str()) != 0) {
        if (auto nameComponent = this->curEnt->tryGetComponent<NameComponent>()) {
            nameComponent->name = std::string(buf);
        } else { // if we don't have the component add it using the contents of buf as the name
            this->curEnt->addComponent<NameComponent>(std::string(buf));
        }
    }

    // we always want to show the uuid since 2+ entities can share a name.
    ImGui::TextDisabled("%s", uuids::to_string(this->curEnt->getUUID()).c_str());

	ImGui::Separator();

    if (ImGui::BeginPopupContextItem("add_component")) {
        // TODO: replace this predefined list with a list of registered components
        std::string complist[18] = [
            "AngelScript",
            "Audio Noise",
            "Audio Sfxr",
            "Audio Speech",
            "Audio Wav",
            "Audio Wav Stream",
            "Camera",
            "Directional Light",
            "Point Light",
            "Spot Light"
            "Mesh",
            "Mesh Dynamic",
            "Name",
            "Skybox",
            "No Render Tag",
            "Scene Tag",
            "Transform"
        ];

        for (std::string compname : complist) {
            ImGui::Selectable("%s Component", compname.c_str());
        }
        ImGui::EndPopup();
    }

    if (ImGui::Button("+"))
        ImGui::OpenPopup("add_component");

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

            ImGui::Text("%s", scriptComp->script->identifier.c_str());
        }

        // Model Dialog specific logic
        scriptDialog.Display();
        if (scriptDialog.HasSelected()) {
            std::string path = FilesystemResourceProvider::getResourceIdentifier(this->modelDialog.GetSelected().string());
            if (!path.empty()) {
                this->curEnt->tryRemoveComponent<AngelScriptComponent>();
                this->curEnt->addComponent<AngelScriptComponent>(path);
            }

            scriptDialog.ClearSelected();
        }
    }
}
