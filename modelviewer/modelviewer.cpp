#include <fstream>

#include <core/engine.h>
#include <input/inputManager.h>
#include <resource/provider/filesystemResourceProvider.h>
#include <i18n/translationManager.h>
#include <entity/model/mesh.h>
#include <entity/model/meshDynamic.h>
#include <entity/camera/editorCamera.h>
#include <entity/gui/panel.h>
#include <utility/dialogs.h>

using namespace chira;

static inline void addResourceFolderSelected() {
    auto folder = dialogOpenFolder();
    if (folder.empty())
        return dialogPopupError(TR("error.modelviewer.resource_folder_not_selected"));

    auto resourceFolderPath = FilesystemResourceProvider::getResourceFolderPath(folder);
    if (resourceFolderPath.empty())
        return dialogPopupError(TR("error.modelviewer.resource_folder_not_valid"));

    bool resourceExists = false;
    for (const auto& fileProvider : Resource::getResourceProviders(FILESYSTEM_PROVIDER_NAME)) {
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

class ModelViewer : public Panel {
public:
    ModelViewer() : Panel(TR("ui.window.title"), true) {
        this->flags |=
                ImGuiWindowFlags_NoTitleBar   |
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoResize     |
                ImGuiWindowFlags_NoBackground ;
    }

    static ModelViewer* get() {
        static auto gui = new ModelViewer{};
        return gui;
    }

    void addModelSelected() {
        std::string path = dialogOpenResource("*.json");
        if (path.empty())
            return dialogPopupError(TR("error.modelviewer.file_is_not_resource"));
        this->setLoadedFile(path);
    }

    void convertToModelTypeSelected(const std::string& extension, const std::string& type) const {
        std::string filepath = dialogSaveFile(extension);
        if (filepath.empty())
            return dialogPopupError(TR("error.modelviewer.filename_empty"));

        if (!Engine::getWindow()->hasChild(this->meshId))
            return dialogPopupError(TR("error.modelviewer.no_model_present"));

        std::ofstream file{filepath, std::ios::binary};
        std::vector<byte> meshData = Engine::getWindow()->getChild<Mesh>(this->meshId)->getMeshData(type);
        file.write(reinterpret_cast<const char*>(&meshData[0]), static_cast<std::streamsize>(meshData.size()));
        file.close();
    }

    void convertToOBJSelected() const {
        this->convertToModelTypeSelected(".obj", "obj");
    }

    void convertToCMDLSelected() const {
        this->convertToModelTypeSelected(".cmdl", "cmdl");
    }

    void preRenderContents() override {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu(TRC("ui.menubar.file"))) { // File
                if (ImGui::MenuItem(TRC("ui.menubar.open_model"))) // Open Model...
                    this->addModelSelected();
                if (ImGui::MenuItem(TRC("ui.menubar.add_resource_folder"))) // Add Resource Folder...
                    addResourceFolderSelected();
                ImGui::Separator();
                if (ImGui::MenuItem(TRC("ui.menubar.exit"))) // Exit
                    this->getWindow()->shouldStopAfterThisFrame();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu(TRC("ui.menubar.convert"))) { // Convert
                if (ImGui::MenuItem(TRC("ui.menubar.convert_to_obj"))) // Convert to OBJ...
                    this->convertToOBJSelected();
                if (ImGui::MenuItem(TRC("ui.menubar.convert_to_cmdl"))) // Convert to CMDL...
                    this->convertToCMDLSelected();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::SetNextWindowPos(ImVec2{0, ImGui::GetFrameHeight()});
        ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()});
    }

    void renderContents() override {
        ImGui::Checkbox(TRC("ui.modelviewer.show_grid"), &this->showGrid);
        Engine::getWindow()->getChild("grid")->setVisible(this->showGrid);
        ImGui::Text("%s", ModelViewer::loadedFile.c_str());
    }

    void setLoadedFile(const std::string& meshName) {
        if (Engine::getWindow()->hasChild(this->meshId) && meshName == Engine::getWindow()->getChild<Mesh>(this->meshId)->getMeshResource()->getIdentifier())
            return;
        if (!Resource::hasResource(meshName)) {
            dialogPopupError(TRF("error.modelviewer.resource_is_invalid", meshName));
            return;
        }
        if (Engine::getWindow()->hasChild(this->meshId))
            Engine::getWindow()->removeChild(this->meshId);
        this->meshId = Engine::getWindow()->addChild(new Mesh{meshName});
        ModelViewer::loadedFile = meshName;
    }

    [[nodiscard]] std::string getMeshId() const {
        return this->meshId;
    }
private:
    std::string loadedFile;
    std::string meshId;
    bool showGrid = true;
};

int main() {
    Engine::preInit("settings_modelviewer.json");
    Resource::addResourceProvider(new FilesystemResourceProvider{"modelviewer"});
    TranslationManager::addTranslationFile("file://i18n/modelviewer");

    Engine::init([]{
        Engine::getWindow()->addChild(ModelViewer::get());
        Engine::getWindow()->setBackgroundColor(ColorRGB{0.15f});

        auto camera = new EditorCamera{CameraProjectionMode::PERSPECTIVE, 120.f};
        camera->translate({-6.f * sqrtf(3.f), 6, 0});
        camera->setPitch(30.f);
        camera->setYaw(270.f);
        Engine::getWindow()->addChild(camera);
        Engine::getWindow()->setCamera(camera);
        EditorCamera::setupKeybinds();

        auto grid = new MeshDynamic{"grid"};
        auto gridMesh = grid->getMesh();
        gridMesh->setMaterial(CHIRA_GET_MATERIAL("MaterialUntextured", "file://materials/unlit.json"));
        for (int i = -5; i <= 5; i++) {
            gridMesh->addCube(Vertex{{i, 0, 0}}, {0.025f, 0.025f, 10.f});
            gridMesh->addCube(Vertex{{0, 0, i}}, {10.f, 0.025f, 0.025f});
        }
        gridMesh->addCube({{2.5f, 0, 0}, {0, 0, 0}, {1, 0, 0}}, {5.f + 0.026f, 0.03f, 0.03f});
        gridMesh->addCube({{0, 0, 2.5f}, {0, 0, 0}, {0, 0, 1}}, {0.03f, 0.03f, 5.f + 0.026f});
        gridMesh->addCube({{0, 0,    0}, {0, 0, 0}, {0, 1, 0}}, glm::vec3{0.05f});
        Engine::getWindow()->addChild(grid);

        auto teapotMesh = Resource::getResource<MeshDataResource>("file://meshes/teapot.json");
        const auto teapotShader = teapotMesh->getMaterial()->getShader();
        teapotShader->use();
        teapotShader->setUniform("light.ambient", 0.1f, 0.1f, 0.1f);
        teapotShader->setUniform("light.diffuse", 1.0f, 1.0f, 1.0f);
        teapotShader->setUniform("light.specular", 1.0f, 1.0f, 1.0f);
        teapotShader->setUniform("light.position", 0.0f, 5.0f, 0.0f);
    });
    Engine::run();
}
