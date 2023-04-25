#include "InspectorPanel.h"

#include <core/Logger.h>

using namespace chira;

CHIRA_CREATE_LOG(INSPECTOR);

InspectorPanel::InspectorPanel()
	: IPanel("Inspector", true) {
	this->flags |=
		ImGuiWindowFlags_None;
}

void InspectorPanel::setEntity(Entity *newEnt) {
	curEnt = newEnt;
}

Entity *InspectorPanel::getEntity() {
	return curEnt;
}

void InspectorPanel::renderContents() {
	ImGui::Text("Entity Name");
	ImGui::Separator();

	// testing
	if (ImGui::CollapsingHeader("Transform")) {
		const float   f32_zero = 0.f, f32_one = 1.f;
		
		static glm::vec3f position;

		ImGui::Text("Position");
		ImGui::Separator();
		ImGui::DragScalar("X", ImGuiDataType_Float,  &position.x, 0.005f,  &f32_zero, &f32_one, "%f");
		ImGui::DragScalar("Y", ImGuiDataType_Float,  &position.y, 0.005f,  &f32_zero, &f32_one, "%f");
		ImGui::DragScalar("Z", ImGuiDataType_Float,  &position.z, 0.005f,  &f32_zero, &f32_one, "%f");
	}

	if (ImGui::CollapsingHeader("Mesh")) {
		ImGui::Text("TODO: This");
		ImGui::Separator();
	}
}
