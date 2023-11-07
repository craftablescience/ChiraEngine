#include "IViewportPanel.h"

#include <entity/Viewport.h>

using namespace chira;

IViewportPanel::IViewportPanel(const std::string& title_, Viewport* viewport_, bool resizeLayer_, bool startVisible, ImVec2 windowSize, bool enforceSize)
    : IPanel(title_, startVisible, windowSize, enforceSize)
    , viewport(viewport_)
    , currentSize(windowSize.x, windowSize.y)
    , shouldResize(resizeLayer_) {}

void IViewportPanel::renderContents() {
    if (ImGui::BeginChild("__internal_frame__")) {
        ImVec2 guiSize = ImGui::GetWindowSize();
        glm::vec2i size{guiSize.x, guiSize.y};
        if (this->currentSize != size) {
            if (this->shouldResize) {
                this->viewport->setSize(size);
            }
            this->currentSize = size;
        }
        ImGui::Image(Renderer::getImGuiFrameBufferHandle(*this->viewport->getRawHandle()), guiSize, ImVec2(0, 1), ImVec2(1, 0));
        this->renderViewportContents();
    }
    ImGui::EndChild();
}
