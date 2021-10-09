#include "abstractUiWindowComponent.h"

#include <imgui.h>

using namespace chira;

abstractUiWindowComponent::abstractUiWindowComponent(const std::string& title_, bool startVisible, const ImVec2& windowSize, bool enforceSize) {
    this->title = title_;
    this->isVisible_ = startVisible;
    this->nextWindowSize = windowSize;
    this->windowSizeCondition = enforceSize? ImGuiCond_Always : ImGuiCond_FirstUseEver;
}

void abstractUiWindowComponent::render(double delta) {
    if (this->isVisible_) {
        ImGui::SetNextWindowSize(this->nextWindowSize, this->windowSizeCondition);
        if (ImGui::Begin(this->title.c_str(), &this->isVisible_)) {
            this->draw(delta);
        }
        ImGui::End();
    }
}

void abstractUiWindowComponent::setVisible(bool visible) {
    this->isVisible_ = visible;
}

bool abstractUiWindowComponent::isVisible() const {
    return this->isVisible_;
}
