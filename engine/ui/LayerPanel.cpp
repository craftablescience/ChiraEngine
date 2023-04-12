#include "LayerPanel.h"

#include <entity/Layer.h>

using namespace chira;

LayerPanel::LayerPanel(const std::string& title_, Layer* layer_, bool startVisible, ImVec2 windowSize, bool enforceSize)
    : IPanel(title_, startVisible, windowSize, enforceSize)
    , layer(layer_)
    , currentSize(windowSize.x, windowSize.y) {}

void LayerPanel::renderContents() {
    if (ImGui::BeginChild("__internal_frame__")) {
        ImVec2 guiSize = ImGui::GetWindowSize();
        glm::vec2i size{guiSize.x, guiSize.y};
        if (this->currentSize != size) {
            this->layer->setSize(size);
            this->currentSize = size;
        }
        ImGui::Image(Renderer::getImGuiFrameBufferHandle(*this->layer->getRawHandle()), guiSize, ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::EndChild();
}
