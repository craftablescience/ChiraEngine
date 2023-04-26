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
    char *buf;

    if (sizeof(this->curEnt->getName().c_str()) < 2048)
        strncpy(buf, this->curEnt->getName().c_str(), 2048);

    ImGui::InputText("##Name", buf, 2048);

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
}
