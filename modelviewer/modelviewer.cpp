#include <fstream>
#include <core/engine.h>
#include <input/inputManager.h>
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
    explicit ModelViewerGui(const std::string_view& meshId_)
        : Window(TR("ui.window.title"), true)
        , meshId(meshId_) {
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
        if (meshName == Engine::getRoot()->getChild<Mesh3d>(this->meshId.data())->getMeshResource()->getIdentifier())
            return;
        Engine::getRoot()->removeChild(this->meshId.data());
        Resource::cleanup();
        this->meshId = Engine::getRoot()->addChild(new Mesh3d{Resource::getResource<MeshResource>(meshName)});
        ModelViewerGui::loadedFile = meshName;
    }
    [[nodiscard]] std::string_view getMeshId() const {
        return this->meshId;
    }
private:
    std::string loadedFile = "file://meshes/editor/grid.json";
    std::string_view meshId;
};

inline void addModelSelected(const std::string_view& modelId) {
    std::string path = dialogOpenResource("*.json");
    if (path.empty())
        return dialogPopupError(TR("error.modelviewer.file_is_not_resource"));
    Engine::getRoot()->getChild<ModelViewerGui>(modelId.data())->setLoadedFile(path);
}

inline void addResourceFolderSelected() {
    auto folder = dialogOpenFolder();
    if (folder.empty())
        return dialogPopupError(TR("error.modelviewer.resource_folder_not_selected"));

    auto resourceFolderPath = FilesystemResourceProvider::getResourceFolderPath(folder);
    if (resourceFolderPath.empty())
        return dialogPopupError(TR("error.modelviewer.resource_folder_not_valid"));

    bool resourceExists = false;
    for (const auto &fileProvider: Resource::getResourceProviders(FILESYSTEM_PROVIDER_NAME)) {
        if (resourceFolderPath == assert_cast<FilesystemResourceProvider*>(fileProvider.get())->getFolder()) {
            resourceExists = true;
            break;
        }
    }
    if (!resourceExists)
        Resource::addResourceProvider(new FilesystemResourceProvider{resourceFolderPath});
    else
        dialogPopupError(TR("error.modelviewer.resource_folder_already_registered"));
}

inline void convertToModelTypeSelected(const std::string& extension, const std::string& type, const std::string_view guiId) {
    std::string filepath = dialogSaveFile(extension);
    if (filepath.empty())
        return;
    std::ofstream file{filepath, std::ios::binary};
    auto meshId = Engine::getRoot()->getChild<ModelViewerGui>(guiId.data())->getMeshId();
    std::vector<byte> meshData = Engine::getRoot()->getChild<Mesh3d>(meshId.data())->getMeshData(type);
    file.write(reinterpret_cast<const char*>(&meshData.front()), static_cast<std::streamsize>(meshData.size()));
}

inline void convertToOBJSelected(const std::string_view guiId) {
    convertToModelTypeSelected(".obj", "obj", guiId);
}

inline void convertToCMDLSelected(const std::string_view guiId) {
    convertToModelTypeSelected(".cmdl", "cmdl", guiId);
}

int main() {
    Engine::preInit("settings_modelviewer.json");
    Resource::addResourceProvider(new FilesystemResourceProvider{"modelviewer"});
    TranslationManager::addTranslationFile("file://i18n/modelviewer");

    // Forcibly overwrite the window size to be smaller
    Engine::getSettingsLoader()->setValue("graphics", "windowWidth", 500, true, false);
    Engine::getSettingsLoader()->setValue("graphics", "windowHeight", 600, true, true);

#ifdef DEBUG
    InputManager::addCallback(InputKeyButton{Key::GRAVE_ACCENT, InputKeyEventType::PRESSED, []{
        Engine::getConsole()->setVisible(!Engine::getConsole()->isVisible());
    }});
    InputManager::addCallback(InputKeyButton{Key::F1, InputKeyEventType::PRESSED, []{
        Engine::getProfiler()->setVisible(!Engine::getProfiler()->isVisible());
    }});
#endif

    std::string_view uiUUID;
    Engine::addInitFunction([&uiUUID]{
        Engine::setBackgroundColor(ColorRGB::solid(0.15f));

        auto camera = new EditorCamera3d{CameraProjectionMode::PERSPECTIVE, 120.f, true};
        Engine::getRoot()->addChild(camera);
        Engine::getRoot()->setMainCamera(camera);

        const auto modelViewerGui = new ModelViewerGui{
            Engine::getRoot()->addChild(new Mesh3d{Resource::getResource<MeshResource>("file://meshes/editor/grid.json")})
        };
        uiUUID = Engine::getRoot()->addChild(modelViewerGui);
    });
    Engine::init();

    Engine::addRenderFunction([&uiUUID]{
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu(TRC("ui.menubar.file"))) { // File
                if (ImGui::MenuItem(TRC("ui.menubar.open_model"))) // Open Model...
                    addModelSelected(uiUUID);
                if (ImGui::MenuItem(TRC("ui.menubar.add_resource_folder"))) // Add Resource Folder...
                    addResourceFolderSelected();
                ImGui::Separator();
                if (ImGui::MenuItem(TRC("ui.menubar.exit"))) // Exit
                    Engine::shouldStopAfterThisFrame(true);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu(TRC("ui.menubar.convert"))) { // Convert
                if (ImGui::MenuItem(TRC("ui.menubar.convert_to_obj"))) // Convert to OBJ...
                    convertToOBJSelected(uiUUID);
                if (ImGui::MenuItem(TRC("ui.menubar.convert_to_cmdl"))) // Convert to CMDL...
                    convertToCMDLSelected(uiUUID);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    });
    Engine::run();
}
