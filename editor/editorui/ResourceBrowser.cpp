#include "ResourceBrowser.h"

#include <fstream>
#include <core/Engine.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <i18n/TranslationManager.h>
#include <ui/IPanel.h>
#include <ui/FramePanel.h>
#include <utility/Dialogs.h>
// Used because file dialogs are broken on macOS
#include <imfilebrowser.h>

using namespace chira;

ResourceBrowser::ResourceBrowser()
    : IPanel(TRC("ui.resourcebrowser.title"), true, ImVec2(2.0F, 2.0F), false), currentSize(2.0F, 2.0F) {
}

void ResourceBrowser::GetMeshList() {
    // STUB
}

void ResourceBrowser::renderContents() {
    ImGui::Button("Testing");
}