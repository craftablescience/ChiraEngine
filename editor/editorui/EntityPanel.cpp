#include "EntityPanel.h"

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

EntityPanel::EntityPanel(Frame* frame)
    : IPanel(TRC("ui.entitypanel.title"), true, ImVec2(2.0F, 2.0F), false), currentSize(2.0F, 2.0F) {
    this->curframe = frame;
}

void EntityPanel::renderContents() {
    ImGui::Text(TRC("ui.entitypanel.scenetree"));
    
    //#region entity table
    if (ImGui::BeginTable("scene_entity_list", 2,
        ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody))
    {
        // Icon column
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_NoHide|ImGuiTableColumnFlags_WidthFixed, 15);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
    
        ImGui::TableHeadersRow();
        
        for (auto const& entity : *this->curframe->getChildren())
        {
            bool is_selected = false;
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TreeNodeEx("", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
            ImGui::TableNextColumn();
            ImGui::Selectable(std::string(entity->getName()).c_str(), is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap);
        }
        
        ImGui::EndTable();
    }
}
