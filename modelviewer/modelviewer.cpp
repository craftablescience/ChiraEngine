#include <core/engine.h>
#include <resource/provider/filesystemResourceProvider.h>
#include <entity/3d/model/mesh3d.h>
#include <i18n/translationManager.h>
#include <entity/3d/camera/freecam.h>
#include <entity/imgui/console/console.h>
#include <entity/imgui/profiler/profiler.h>
#include <utility/dialogs.h>

// Register all materials with the material factory
#include <utility/materialTypes.h>

using namespace chira;

class modelViewerGui : public window {
public:
    modelViewerGui() : window(TR("ui.window.title"), true) {
        this->flags |=
                ImGuiWindowFlags_NoTitleBar   |
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoResize     |
                ImGuiWindowFlags_NoBackground ;
    }
    void preRenderContents() override {
        ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()));
    }
    void renderContents() override {
        ImGui::Text("%s", modelViewerGui::loadedFile.c_str());
    }
    void setLoadedFile(const std::string& meshName) {
        if (meshName == dynamic_cast<mesh3d*>(engine::getRoot()->getChild("modelViewerMesh"))->getMeshResource()->getIdentifier())
            return;
        engine::getRoot()->removeChild("modelViewerMesh");
        engine::getRoot()->addChild(new mesh3d{"modelViewerMesh", resource::getResource<meshResource>(meshName)});
        modelViewerGui::loadedFile = meshName;
    }
private:
    std::string loadedFile;
};

int main() {
    engine::preInit("settings_modelviewer.json");
    resource::addResourceProvider(new filesystemResourceProvider{"editor"});
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

    std::string_view uiUUID;
    engine::addInitFunction([&uiUUID]() {
        engine::setBackgroundColor(colorRGB::solid(0.15f));

        engine::captureMouse(true);
        auto camera = new freecam{cameraProjectionMode::PERSPECTIVE, 120.f, true};
        engine::getRoot()->addChild(camera);
        engine::getRoot()->setMainCamera(camera);

        uiUUID = engine::getRoot()->addChild(new modelViewerGui{});
        auto gridMesh = resource::getResource<meshResource>("file://meshes/editor/grid.json");
        engine::getRoot()->addChild(new mesh3d{"modelViewerMesh", gridMesh});

        // todo: make this an engine function
        glfwSetWindowAspectRatio(engine::getWindow(), 500, 600);
    });
    engine::init();

    engine::addRenderFunction([&uiUUID]() {
        // todo(i18n)
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open...")) {
                    std::string path = dialogOpenResource("*.json");
                    if (!path.empty()) {
                        dynamic_cast<modelViewerGui*>(engine::getRoot()->getChild(uiUUID.data()))->setLoadedFile(path);
                    } else
                        dialogPopupError("File selected is not a resource!");
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
