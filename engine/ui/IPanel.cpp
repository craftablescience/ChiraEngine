#include "IPanel.h"

using namespace chira;

IPanel::IPanel(std::string title_, bool startVisible, ImVec2 windowSize, bool enforceSize)
    : title(std::move(title_))
    , visible(startVisible)
    , activatedThisFrame(startVisible)
    , nextWindowSize(windowSize)
    , windowSizeCondition(enforceSize ? ImGuiCond_Always : ImGuiCond_FirstUseEver)
    , flags(0) {}

void IPanel::render() {
    if (this->visible) {
        ImGui::SetNextWindowSize(this->nextWindowSize, this->windowSizeCondition);
        this->preRenderContents();
        if (ImGui::Begin(this->title.c_str(), &this->visible, this->flags)) {
            this->renderContents();
        }
        ImGui::End();
        this->postRenderContents();

        if (this->activatedThisFrame)
            this->activatedThisFrame = false;
    }
}

std::string_view IPanel::getTitle() const {
    return this->title;
}

void IPanel::setTitle(const std::string& newTitle) {
    this->title = newTitle;
}

bool IPanel::isVisible() const {
    return this->visible;
}

void IPanel::setVisible(bool visible_) {
    if (visible_)
        this->activatedThisFrame = true;
    this->visible = visible_;
}

bool IPanel::wasActivatedThisFrame() const {
    return this->activatedThisFrame;
}

glm::vec2 IPanel::getWindowSize() const {
    return {this->nextWindowSize.x, this->nextWindowSize.y};
}

ImGuiWindowFlags& IPanel::getWindowFlags() {
    return this->flags;
}

void IPanel::setWindowSize(glm::vec2 size) {
    this->nextWindowSize.x = size.x;
    this->nextWindowSize.y = size.y;
}
