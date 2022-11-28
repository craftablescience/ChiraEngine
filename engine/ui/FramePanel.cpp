#include "FramePanel.h"

#include <entity/root/Frame.h>
#include <core/Engine.h>

using namespace chira;

FramePanel::FramePanel(const std::string& title_, bool startVisible, ImVec2 windowSize, bool enforceSize)
    : IPanel(title_, startVisible, windowSize, enforceSize), currentSize(windowSize.x, windowSize.y) {
    this->frame = new Frame{static_cast<int>(windowSize.x), static_cast<int>(windowSize.y)};
}

FramePanel::~FramePanel() {
    delete this->frame;
}

void FramePanel::renderContents() {
    this->frame->update();
    this->frame->render(Engine::getWindow()->getFramebufferHandle(),
                        Engine::getWindow()->getFrameSize().x,
                        Engine::getWindow()->getFrameSize().y);

    this->renderPreFrameContents();
    if (ImGui::BeginChild("__internal_frame__")) {
        ImVec2 guiSize = ImGui::GetWindowSize();
        glm::vec2i size{guiSize.x, guiSize.y};
        if (this->currentSize != size) {
            this->frame->setFrameSize(size);
            this->currentSize = size;
        }
        // thank you C++ for this whole casting mess
        ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<std::intptr_t>(this->frame->getColorTextureHandle())), guiSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::SameLine();
        ImGui::SetCursorPos(ImGui::GetCursorStartPos());
        if (ImGui::BeginChild("__internal_frame_overlay__")) {
            ImVec2 guiSize = ImGui::GetWindowSize();
            glm::vec2i size{guiSize.x, guiSize.y};
            if (this->currentSize != size) {
                this->frame->setFrameSize(size);
                this->currentSize = size;
            }
            this->renderOverlayContents();
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();
    this->renderPostFrameContents();
}

// Stub methods to shut up compiler
void FramePanel::renderOverlayContents() {}
void FramePanel::renderPostFrameContents() {}
void FramePanel::renderPreFrameContents() {}

Frame* FramePanel::getFrame() const {
    return this->frame;
}
