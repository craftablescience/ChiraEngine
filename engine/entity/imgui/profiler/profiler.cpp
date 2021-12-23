#include "profiler.h"

#include <resource/resource.h>
#include <i18n/translationManager.h>

using namespace chira;

profiler::profiler(const ImVec2& windowSize) : window(TR("ui.profiler.title"), false, windowSize) {}

void profiler::renderContents() {
    if (ImGui::BeginTable("Resources", 3)) {
        for (const auto& [providerName, resourceMap] : resource::resources) {
            for (const auto& [name, resource] : resourceMap) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", providerName.c_str());
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", name.c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%d", resource.useCount());
            }
        }
        ImGui::EndTable();
    }
}
