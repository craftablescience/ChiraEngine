#include <core/Engine.h>
#include <core/Logger.h>
#include <i18n/TranslationManager.h>
#include <resource/Resource.h>
#include <ui/IPanel.h>

#include "../ToolHelpers.h"
#include "render/texture/Texture.h"

using namespace chira;

CHIRA_SETUP_GUI_TOOL(PROJECTMANAGER);

static void drawProjectItem(std::string_view title, std::string_view description, void* logo = nullptr) {
    static auto defaultLogo = Resource::getResource<Texture>("file://textures/ui/icon.json");
    ImGui::Image(logo ? logo : defaultLogo->getImGuiTextureHandle(), ImVec2(100, 100));

    ImGui::SameLine();

    ImGui::BeginGroup();

    ImGui::Text("%s", title.data());
    ImGui::TextWrapped("%s", description.data());

    ImGui::EndGroup();

    ImGui::SameLine();

    if (ImGui::Button("Open")) {
    }
}

class ProjectManagerPanel : public IPanel {
public:
    ProjectManagerPanel() : IPanel("Project Manager", true) {
        this->flags |=
                ImGuiWindowFlags_NoTitleBar   |
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoResize     |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoBackground;
    }

    void preRenderContents() override {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu(TRC("ui.menubar.file"))) { // File
                if (ImGui::MenuItem(TRC("ui.menubar.exit"))) // Exit
                    Device::queueDestroyWindow(Engine::getMainWindow(), true);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::SetNextWindowPos(ImVec2{0, ImGui::GetFrameHeight()});
        ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()});
    }

    void renderContents() override {
        drawProjectItem("Project 1", "Lorem ipsum");
        drawProjectItem("Project 2", "Lorem ipsum");
        drawProjectItem("Project 3", "Lorem ipsum");
    }
};

int main(int argc, char* argv[]) {
    Engine::preinit(argc, argv);
    Engine::init();

    auto* window = Engine::getMainWindow();
    auto* viewport = Device::getWindowViewport(window);
    viewport->setBackgroundColor({0.15f});

    auto panel = new ProjectManagerPanel{};
    Device::addPanelToWindow(window, panel);

    Engine::run();
}
