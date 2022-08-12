// Disable console window on Windows (MSVC)
#include <core/Platform.h>
#if defined(_WIN32) && !defined(DEBUG) && defined(CHIRA_COMPILER_MSVC)
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <fstream>
#include <config/ConEntry.h>
#include <core/Engine.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <i18n/TranslationManager.h>
#include <entity/model/Mesh.h>
#include <entity/model/MeshDynamic.h>
#include <entity/camera/EditorCamera.h>
#include <ui/IPanel.h>
#include <utility/Dialogs.h>

#ifdef CHIRA_USE_DISCORD
    #include <hook/DiscordRPC.h>
#endif
#ifdef CHIRA_USE_STEAMWORKS
    #include <hook/SteamAPI.h>
#endif

using namespace chira;

static inline void addResourceFolderSelected() {
    auto folder = Dialogs::openFolder();
    if (folder.empty())
        return Dialogs::popupError(TR("error.modelviewer.resource_folder_not_selected"));

    auto resourceFolderPath = FilesystemResourceProvider::getResourceFolderPath(folder);
    if (resourceFolderPath.empty())
        return Dialogs::popupError(TR("error.modelviewer.resource_folder_not_valid"));

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
        Dialogs::popupError(TR("error.modelviewer.resource_folder_already_registered"));
}

class ModelViewerPanel : public IPanel {
public:
    ModelViewerPanel() : IPanel(TR("ui.window.title"), true) {
        this->flags |=
                ImGuiWindowFlags_NoTitleBar   |
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoResize     |
                ImGuiWindowFlags_NoBackground ;
    }

    void addModelSelected() {
        std::string path = Dialogs::openResource("*.json");
        if (path.empty())
            return Dialogs::popupError(TR("error.modelviewer.file_is_not_resource"));
        this->setLoadedFile(path);
    }

    void convertToModelTypeSelected(const std::string& extension, const std::string& type) const {
        std::string filepath = Dialogs::saveFile(extension);
        if (filepath.empty())
            return Dialogs::popupError(TR("error.modelviewer.filename_empty"));

        if (!Engine::getWindow()->hasChild(this->meshId))
            return Dialogs::popupError(TR("error.modelviewer.no_model_present"));

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
                    Engine::getWindow()->shouldStopAfterThisFrame();
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
        ImGui::Checkbox(TRC("ui.editor.show_grid"), &this->showGrid);
        Engine::getWindow()->getChild("grid")->setVisible(this->showGrid);
        ImGui::Text("%s", this->loadedFile.c_str());
    }

    void setLoadedFile(const std::string& meshName) {
        if (Engine::getWindow()->hasChild(this->meshId) && meshName == Engine::getWindow()->getChild<Mesh>(this->meshId)->getMeshResource()->getIdentifier())
            return;
        if (!Resource::hasResource(meshName)) {
            Dialogs::popupError(TRF("error.modelviewer.resource_is_invalid", meshName));
            return;
        }
        if (Engine::getWindow()->hasChild(this->meshId))
            Engine::getWindow()->removeChild(this->meshId);
        this->meshId = Engine::getWindow()->addChild(new Mesh{meshName});
        this->loadedFile = meshName;
    }

    [[nodiscard]] std::string_view getMeshId() const {
        return this->meshId;
    }
private:
    std::string loadedFile;
    std::string meshId;
    bool showGrid = true;
};

int main(int argc, const char* const argv[]) {
    Engine::preInit(argc, argv);
    Resource::addResourceProvider(new FilesystemResourceProvider{"editor"});
    TranslationManager::addTranslationFile("file://i18n/editor");
    TranslationManager::addUniversalFile("file://i18n/editor");

#ifdef CHIRA_USE_DISCORD
    DiscordRPC::init(TR("editor.discord.application_id"));
    DiscordRPC::setLargeImage("main_logo");
    DiscordRPC::setState("https://discord.gg/ASgHFkX");
#endif

#if defined(CHIRA_USE_STEAMWORKS) && defined(DEBUG)
    if (ConVarRegistry::hasConVar("steam_enable") && ConVarRegistry::getConVar("steam_enable")->getValue<bool>()) {
        // Steam API docs say this is bad practice, I say I don't care
        SteamAPI::generateAppIDFile(1728950);
    }
#endif

    Engine::init();

    Engine::getWindow()->setBackgroundColor(ColorRGB{0.15f});

    Engine::getWindow()->addPanel(new ModelViewerPanel{});

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
        gridMesh->addCube(Vertex{{i, 0, 0}}, {0.025f, 0.025f, 10.025f});
        gridMesh->addCube(Vertex{{0, 0, i}}, {10.025f, 0.025f, 0.025f});
    }
    gridMesh->addCube({{2.5f, 0, 0}, {0, 0, 0}, {1, 0, 0}}, {5.f + 0.026f, 0.03f, 0.03f});
    gridMesh->addCube({{0, 0, 2.5f}, {0, 0, 0}, {0, 0, 1}}, {0.03f, 0.03f, 5.f + 0.026f});
    gridMesh->addCube({{0, 0,    0}, {0, 0, 0}, {0, 1, 0}}, glm::vec3{0.05f});
    Engine::getWindow()->addChild(grid);

    Engine::run();
}
