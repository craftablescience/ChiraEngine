#include "profiler.h"

#include <i18n/translationManager.h>

using namespace chira;

Profiler::Profiler(ImVec2 windowSize) : Panel(TR("ui.profiler.title"), false, windowSize) {}

void Profiler::renderContents() {
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
