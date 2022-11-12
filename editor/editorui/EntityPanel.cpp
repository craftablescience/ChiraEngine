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
            bool op = false;
            bool is_selected = false;
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TreeNodeEx("", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
            ImGui::TableNextColumn();
            ImGui::TreeNodeEx(std::string(entity->getName()).c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::Selectable("Delete")) {
                    // properly destroy the entity
                    this->curframe->removeChild(entity->getName());
                }
                if (ImGui::Selectable("Rename")) {
                    op = true;
                }

                ImGui::EndPopup();
            }
            if (op)
                ImGui::OpenPopup(std::string("Rename##" + std::string(entity->getName())).c_str());

            // Always center this window when appearing
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            // Popup window for renaming
            if (ImGui::BeginPopupModal(std::string("Rename##" + std::string(entity->getName())).c_str(), NULL, 0)) {
                ImGui::Text("New Name:");
                
                static char buf1[64] = ""; ImGui::InputText(std::string("##" + std::string(entity->getName())).c_str(),     buf1, 64);
                ImGui::Text(buf1);

                if (ImGui::Button("Ok")) {
                    entity->setName(std::string(buf1));
                    memset(buf1, 0, sizeof(buf1));
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) {
                    memset(buf1, 0, sizeof(buf1));
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
        
        ImGui::EndTable();
    }
}

