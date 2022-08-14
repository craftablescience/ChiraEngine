#include "FramePanel.h"

#include <entity/root/Frame.h>
#include <core/Engine.h>

using namespace chira;

FramePanel::FramePanel(const std::string& title_, bool startVisible, ImVec2 windowSize, bool enforceSize)
    : IPanel(title_, startVisible, windowSize, enforceSize), currentSize(windowSize.x, windowSize.y) {
    this->frame = new Frame{static_cast<int>(windowSize.x), static_cast<int>(windowSize.y)};
    Engine::getWindow()->addChild(this->frame);
}

FramePanel::~FramePanel() {
    Engine::getWindow()->removeChild(this->frame->getName());
    delete this->frame;
}

void FramePanel::renderContents() {
    this->frame->render(glm::identity<glm::mat4>());
    if (ImGui::BeginChild("__internal_frame__")) {
        ImVec2 guiSize = ImGui::GetWindowSize();
        glm::vec2i size{guiSize.x, guiSize.y};
        if (this->currentSize != size) {
            this->frame->setFrameSize(size);
            this->currentSize = size;
        }
        // thank you C++ for this whole casting mess
        ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<unsigned long long>(this->frame->getColorTextureHandle())), guiSize, ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::EndChild();
}

// Proxy to add a child to the frame
std::string_view FramePanel::addChild(chira::Entity *child) {
    return this->frame->addChild(child);
}

// Proxy to remove a child from the frame
void FramePanel::removeChild(std::string_view name_) {
    this->frame->removeChild(name_);
}

bool FramePanel::hasChild(std::string_view name_) {
    return this->frame->hasChild(name_);
}

Entity* FramePanel::getChild(std::string_view name_) {
    return this->frame->getChild(name_);
}

void FramePanel::setCamera(Camera* camera) {
    this->frame->setCamera(camera);
}