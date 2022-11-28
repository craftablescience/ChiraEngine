#include "ResourceUsageTrackerPanel.h"

#include <i18n/TranslationManager.h>

using namespace chira;

ResourceUsageTrackerPanel::ResourceUsageTrackerPanel(ImVec2 windowSize) : IPanel(TR("ui.resource_usage_tracker.title"), false, windowSize) {}

void ResourceUsageTrackerPanel::renderContents() {
    if (ImGui::BeginTable("Default Resources", 2)) {
        for (const auto& [resourceHash, resource]: Resource::defaultResources) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", resource->getIdentifier().data());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", resource.useCount());
        }
        ImGui::EndTable();
    }
    ImGui::Separator();
    if (ImGui::BeginTable("Resources", 3)) {
        for (const auto& [providerName, resourceMap] : Resource::resources) {
            for (const auto& [resourceName, resource] : resourceMap) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", providerName.c_str());
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", resourceName.c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%d", resource.useCount());
            }
        }
        ImGui::EndTable();
    }
}
