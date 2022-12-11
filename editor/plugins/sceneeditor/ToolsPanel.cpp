#include "ToolsPanel.h"

#include <fstream>
#include <core/Engine.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <i18n/TranslationManager.h>
#include <ui/IPanel.h>
#include <ui/FramePanel.h>

using namespace chira;

ToolsPanel::ToolsPanel(Frame* frame)
    : IPanel("tools", true, ImVec2(2.0F, 2.0F), false), currentSize(2.0F, 2.0F) {
    this->curframe = frame;
}

void ToolsPanel::renderContents() {
}
    