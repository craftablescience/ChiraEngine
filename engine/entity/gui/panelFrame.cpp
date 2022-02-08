#include "panelFrame.h"

#include <entity/root/frame.h>

using namespace chira;

PanelFrame::PanelFrame(const std::string& title_, bool startVisible, ImVec2 windowSize, bool enforceSize)
    : Panel(title_, startVisible, windowSize, enforceSize), currentSize(windowSize.x, windowSize.y) {
    this->frame = new Frame{static_cast<int>(windowSize.x), static_cast<int>(windowSize.y)};
    this->addChild(this->frame);
}

void PanelFrame::renderContents() {
    ImGui::BeginChild("__internal_frame__");
    ImVec2 guiSize = ImGui::GetWindowSize();
    glm::vec<2, int> size{guiSize.x, guiSize.y};
    if (this->currentSize != size) {
        this->frame->setFrameSize(size);
        this->currentSize = size;
    }
    ImGui::Image(reinterpret_cast<ImTextureID>(this->frame->getColorTextureHandle()), guiSize, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::EndChild();
}

const Frame* PanelFrame::getFrame() const {
    return this->frame;
}

Frame* PanelFrame::getFrame() {
    return this->frame;
}

const Root* PanelFrame::getRoot() const {
    return this->frame;
}

Root* PanelFrame::getRoot() {
    return this->frame;
}
