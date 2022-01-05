#include <core/engine.h>
#include <resource/provider/filesystemResourceProvider.h>
#include <entity/3d/model/mesh3d.h>
#include <i18n/translationManager.h>
#include <entity/3d/camera/editorCamera3d.h>
#include <entity/imgui/console/console.h>
#include <entity/imgui/profiler/profiler.h>
#include <utility/dialogs.h>

using namespace chira;

class ModelViewerGui : public Window {
public:
    ModelViewerGui() : Window(TR("ui.window.title"), true) {
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
        ImGui::Text("%s", ModelViewerGui::loadedFile.c_str());
    }
    void setLoadedFile(const std::string& meshName) {
        if (meshName == assert_cast<Mesh3d*>(Engine::getRoot()->getChild("modelViewerMesh"))->getMeshResource()->getIdentifier())
            return;
        Engine::getRoot()->removeChild("modelViewerMesh");
        Engine::getRoot()->addChild(new Mesh3d{"modelViewerMesh", Resource::getResource<MeshResource>(meshName)});
        ModelViewerGui::loadedFile = meshName;
    }
private:
    std::string loadedFile = "file://meshes/editor/grid.json";
};

int main() {
    Engine::preInit("settings_modelviewer.json");
    Resource::addResourceProvider(new FilesystemResourceProvider{"modelviewer"});
    TranslationManager::addTranslationFile("file://i18n/modelviewer");

    // Forcibly overwrite the window size to be smaller
    Engine::getSettingsLoader()->setValue("graphics", "windowWidth", 500, true, false);
    Engine::getSettingsLoader()->setValue("graphics", "windowHeight", 600, true, true);

#ifdef DEBUG
    Engine::addKeybind(Keybind(GLFW_KEY_GRAVE_ACCENT, GLFW_PRESS, []{
        Engine::getConsole()->setVisible(!Engine::getConsole()->isVisible());
    }));
    Engine::addKeybind(Keybind(GLFW_KEY_F1, GLFW_PRESS, []{
        Engine::getProfiler()->setVisible(!Engine::getProfiler()->isVisible());
    }));
#endif

    std::string_view uiUUID;
    Engine::addInitFunction([&uiUUID]{
        Engine::setBackgroundColor(ColorRGB::solid(0.15f));

        auto camera = new EditorCamera3d{CameraProjectionMode::PERSPECTIVE, 120.f, true};
        Engine::getRoot()->addChild(camera);
        Engine::getRoot()->setMainCamera(camera);

        uiUUID = Engine::getRoot()->addChild(new ModelViewerGui{});
        auto gridMesh = Resource::getResource<MeshResource>("file://meshes/editor/grid.json");
        Engine::getRoot()->addChild(new Mesh3d{"modelViewerMesh", gridMesh});

        // todo: abstract this somehow
        glfwSetWindowAspectRatio(Engine::getWindow(), 500, 600);
    });
    Engine::init();

    Engine::addRenderFunction([&uiUUID]{
        // todo(i18n)
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Model...")) {
                    std::string path = dialogOpenResource("*.json");
                    if (!path.empty()) {
                        assert_cast<ModelViewerGui*>(Engine::getRoot()->getChild(uiUUID.data()))->setLoadedFile(path);
                    } else
                        dialogPopupError("File selected is not a resource!");
                }
                if (ImGui::MenuItem("Add Resource Path...")) {
                    // todo: use folder picker
                    // todo: check if provider already exists
                    Resource::addResourceProvider(new FilesystemResourceProvider{dialogInput("Enter folder name:")});
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {
                    // todo: make this an engine function
                    glfwSetWindowShouldClose(Engine::getWindow(), GLFW_TRUE);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    });
    Engine::run();
}
