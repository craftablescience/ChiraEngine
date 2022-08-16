#include "EngineView.h"

#include <ui/FramePanel.h>
#include <entity/root/Frame.h>
#include <i18n/TranslationManager.h>

using namespace chira;

EngineView::EngineView() 
    : FramePanel(TRC("ui.engineview.title"), true, ImVec2(2.0F, 2.0F), false) {
}

void EngineView::renderOverlayContents() {
    ImGui::Checkbox(TRC("ui.editor.show_grid"), &this->showGrid);
    this->frame->getChild("grid")->setVisible(this->showGrid);
    ImGui::Text("%s", this->loadedFile.c_str());
}