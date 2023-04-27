#include "ILayerPanel.h"

#include <entity/Layer.h>

using namespace chira;

ILayerPanel::ILayerPanel(const std::string& title_, Layer* layer_, bool resizeLayer_, bool startVisible, ImVec2 windowSize, bool enforceSize)
    : IPanel(title_, startVisible, windowSize, enforceSize)
    , layer(layer_)
    , currentSize(windowSize.x, windowSize.y)
    , shouldResize(resizeLayer_) {}

void ILayerPanel::renderContents() {
    if (ImGui::BeginChild("__internal_frame__")) {
        ImVec2 guiSize = ImGui::GetWindowSize();
        glm::vec2i size{guiSize.x, guiSize.y};
        if (this->currentSize != size) {
            if (this->shouldResize) {
                this->layer->setSize(size);
            }
            this->currentSize = size;
        }
        ImGui::Image(Renderer::getImGuiFrameBufferHandle(*this->layer->getRawHandle()), guiSize, ImVec2(0, 1), ImVec2(1, 0));
        this->renderLayerContents();
    }
    ImGui::EndChild();
}
