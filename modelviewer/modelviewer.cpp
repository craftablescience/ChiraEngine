#include <core/engine.h>
#include <resource/provider/filesystemResourceProvider.h>
#include <entity/3d/model/mesh3d.h>
#include <i18n/translationManager.h>
#include <entity/3d/camera/freecam.h>
#include <entity/imgui/console/console.h>
#include <entity/imgui/profiler/profiler.h>
#include <utility/dialogs.h>

using namespace chira;

class modelViewerGui : public window {
public:
    modelViewerGui() : window(TR("ui.window.title"), true) {
        this->flags |=
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoBackground;
    }
    void preRenderContents() override {
        ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()));
    }
    void renderContents() override {
        ImGui::Text("lol");
    }
};

bool openJSONFile(std::string& out) {
    std::string path = openResource("*.json");
    if (!path.empty()) {
        out = path;
        return true;
    }
    logger::log(INFO, "File Picker", TR("generic.operation.cancelled"));
    return false;
}

void loadMesh(const std::string& path) {
    logger::log(INFO_IMPORTANT, "path", path);
}

int main() {
    engine::preInit("settings_cmdlcompiler.json");
    resource::addResourceProvider(new filesystemResourceProvider{"modelviewer"});
    translationManager::addTranslationFile("file://i18n/modelviewer");

    // Forcibly overwrite the window size to be smaller
    engine::getSettingsLoader()->setValue("graphics", "windowWidth", 500, true, false);
    engine::getSettingsLoader()->setValue("graphics", "windowHeight", 600, true, true);

#ifdef DEBUG
    engine::addKeybind(keybind(GLFW_KEY_GRAVE_ACCENT, GLFW_PRESS, []() {
        engine::getConsole()->setVisible(!engine::getConsole()->isVisible());
    }));
    engine::addKeybind(keybind(GLFW_KEY_F1, GLFW_PRESS, []() {
        engine::getProfiler()->setVisible(!engine::getProfiler()->isVisible());
    }));
#endif

    engine::addInitFunction([]() {
        //auto noto = resource::getResource<fontResource>("file://fonts/default.json");
        //ImGui::GetIO().FontDefault = noto->getFont();

        auto camera = new freecam{cameraProjectionMode::PERSPECTIVE, false};
        engine::getRoot()->addChild(camera);
        engine::getRoot()->setMainCamera(camera);

        engine::getRoot()->addChild(new modelViewerGui{});

        // todo: make this an engine function
        glfwSetWindowAspectRatio(engine::getWindow(), 500, 600);
    });
    engine::init();

    engine::addRenderFunction([]() {
        // todo(i18n)
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Model")) {
                    std::string path;
                    openJSONFile(path);
                    loadMesh(path);
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {
                    // todo: make this an engine function
                    glfwSetWindowShouldClose(engine::getWindow(), GLFW_TRUE);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    });
    engine::run();
}
