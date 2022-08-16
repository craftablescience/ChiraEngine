#include "EngineView.h"

#include <ui/FramePanel.h>
#include <i18n/TranslationManager.h>

using namespace chira;

EngineView::EngineView() 
    : FramePanel(TRC("ui.engineview.title"), true, ImVec2(2.0F, 2.0F), false) {
}