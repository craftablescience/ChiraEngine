// Disable console window on Windows (MSVC)
#include <core/Platform.h>
#if defined(CHIRA_PLATFORM_WINDOWS) && !defined(DEBUG) && defined(CHIRA_COMPILER_MSVC)
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <fstream>
#include <config/ConEntry.h>
#include <core/Engine.h>
#include <entity/Entity.h>
#include <entity/component/CameraComponent.h>
#include <entity/component/MeshComponent.h>
#include <entity/component/MeshDynamicComponent.h>
#include <input/InputManager.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <i18n/TranslationManager.h>
#include <ui/IPanel.h>
#include <ui/Popups.h>

#include <imfilebrowser.h>

#ifdef CHIRA_USE_DISCORD
    #include <plugin/DiscordRPC.h>
#endif
#ifdef CHIRA_USE_STEAMWORKS
    #include <plugin/SteamAPI.h>
#endif

// Need to register phong material!
#include <render/material/MaterialPhong.h>

using namespace chira;

class ModelViewerPanel : public IPanel {
public:
    explicit ModelViewerPanel(Scene* scene_)
            : IPanel(TR("ui.engineview.title"), true)
            , scene(scene_) {
        this->flags |=
                ImGuiWindowFlags_NoTitleBar   |
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_NoResize     |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoBackground ;

        this->modelDialog.SetTitle("Open Resource");
        this->modelDialog.SetTypeFilters({".json"});

        this->folderDialog.SetTitle("Add Resource Folder");
    }

    // Opens a file dialog used to select a model definition
    void addModelSelected() {
        std::string path = FilesystemResourceProvider::getResourceIdentifier(this->modelDialog.GetSelected().string());
        if (!path.empty())
            this->setLoadedFile(path);
    }

    void addResourceFolderSelected() {
        std::string resourceFolderPath = FilesystemResourceProvider::getResourceFolderPath(this->modelDialog.GetSelected().string());
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

    void convertToModelTypeSelected(const std::string& filepath, const std::string& type) const {
        if (!scene->hasEntity(this->meshId))
            return Dialogs::popupError(TR("error.modelviewer.no_model_present"));

        std::ofstream file{filepath, std::ios::binary};
        std::vector<byte> meshData = scene->getEntity(this->meshId)->getComponent<MeshComponent>().getMeshData(type);
        file.write(reinterpret_cast<const char*>(meshData.data()), static_cast<std::streamsize>(meshData.size()));
        file.close();
    }

    void convertToOBJSelected() const {
        std::string filepath = this->saveDialogOBJ.GetSelected().string();
        if (filepath.empty())
            return Dialogs::popupError(TR("error.modelviewer.filename_empty"));
        this->convertToModelTypeSelected(filepath, "obj");
    }

    void convertToCMDLSelected() const {
        std::string filepath = this->saveDialogCMDL.GetSelected().string();
        if (filepath.empty())
            return Dialogs::popupError(TR("error.modelviewer.filename_empty"));
        this->convertToModelTypeSelected(filepath, "cmdl");
    }

    void preRenderContents() override {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu(TRC("ui.menubar.file"))) { // File
                if (ImGui::MenuItem(TRC("ui.menubar.open_model"))) // Open Model...
                    this->modelDialog.Open();
                if (ImGui::MenuItem(TRC("ui.menubar.add_resource_folder"))) // Add Resource Folder...
                    this->folderDialog.Open();
                ImGui::Separator();
                if (ImGui::MenuItem(TRC("ui.menubar.exit"))) // Exit
                    Device::queueDestroyWindow(Engine::getMainWindow(), true);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu(TRC("ui.menubar.convert"))) { // Convert
                if (ImGui::MenuItem(TRC("ui.menubar.convert_to_obj"))) // Convert to OBJ...
                    this->saveDialogOBJ.Open();
                if (ImGui::MenuItem(TRC("ui.menubar.convert_to_cmdl"))) // Convert to CMDL...
                    this->saveDialogCMDL.Open();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // Model Dialog specific logic
        this->modelDialog.Display();
        if (this->modelDialog.HasSelected()) {
            this->addModelSelected();
            this->modelDialog.ClearSelected();
        }
        this->folderDialog.Display();
        if (this->folderDialog.HasSelected()) {
            this->addResourceFolderSelected();
            this->folderDialog.ClearSelected();
        }
        this->saveDialogOBJ.Display();
        if (this->saveDialogOBJ.HasSelected()) {
            this->convertToOBJSelected();
            this->saveDialogOBJ.ClearSelected();
        }
        this->saveDialogCMDL.Display();
        if (this->saveDialogCMDL.HasSelected()) {
            this->convertToCMDLSelected();
            this->saveDialogCMDL.ClearSelected();
        }

        ImGui::SetNextWindowPos(ImVec2{0, ImGui::GetFrameHeight()});
        ImGui::SetNextWindowSize(ImVec2{ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()});
    }

    void renderContents() override {
        ImGui::Checkbox(TRC("ui.editor.show_grid"), &this->showGrid);
        // todo(editor): bring back show/hide grid
        //Engine::getRoot()->getChild("grid")->setVisible(this->showGrid);
        ImGui::Text("%s", this->loadedFile.c_str());
    }

    void setLoadedFile(const std::string& meshName) {
        if (scene->hasEntity(this->meshId) && meshName == scene->getEntity(this->meshId)->getComponent<MeshComponent>().getMeshResource()->getIdentifier())
            return;
        if (!Resource::hasResource(meshName)) {
            Dialogs::popupError(TRF("error.modelviewer.resource_is_invalid", meshName));
            return;
        }
        if (!scene->hasEntity(this->meshId)) {
            auto* entity = scene->addEntity();
            this->meshId = entity->getUUID();
        }
        auto* entity = scene->getEntity(this->meshId);
        entity->tryRemoveComponent<MeshComponent>();
        entity->addComponent<MeshComponent>(meshName);
        this->loadedFile = meshName;
    }

    [[nodiscard]] uuids::uuid getMeshId() const {
        return this->meshId;
    }
private:
    Scene* scene;
    std::string loadedFile;
    uuids::uuid meshId;
    bool showGrid = true;
    ImGui::FileBrowser modelDialog{ImGuiFileBrowserFlags_CloseOnEsc};
    ImGui::FileBrowser folderDialog{ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_SelectDirectory};
    ImGui::FileBrowser saveDialogOBJ{ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_EnterNewFilename};
    ImGui::FileBrowser saveDialogCMDL{ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_EnterNewFilename};
};

static void setupKeybinds(TransformComponent& cameraTransform) {
    constexpr float movementSpeed    = 4.f;
    constexpr float mouseSensitivity = 6.f;
    constexpr float scrollModifier   = 10.f;

    Input::KeyEvent::create(Input::Key::SDLK_w, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({0, 0, -movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000)});
    });
    Input::KeyEvent::create(Input::Key::SDLK_s, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({0, 0, movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000)});
    });
    Input::KeyEvent::create(Input::Key::SDLK_a, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({-movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0, 0});
    });
    Input::KeyEvent::create(Input::Key::SDLK_d, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0, 0});
    });
    Input::KeyEvent::create(Input::Key::SDLK_SPACE, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({0, movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0});
    });
    Input::KeyEvent::create(Input::Key::SDLK_LSHIFT, Input::KeyEventType::REPEATED, [&cameraTransform] {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({0, -movementSpeed * (static_cast<float>(Engine::getDeltaTicks()) / 1000), 0});
    });
    Input::MouseEvent::create(Input::Mouse::BUTTON_RIGHT, Input::MouseEventType::CLICKED, [](int, int, uint8_t) {
        Device::setMouseCapturedWindow(Engine::getMainWindow(), true);
    });
    Input::MouseEvent::create(Input::Mouse::BUTTON_RIGHT, Input::MouseEventType::RELEASED, [](int, int, uint8_t) {
        Device::setMouseCapturedWindow(Engine::getMainWindow(), false);
    });
    Input::MouseMotionEvent::create(Input::MouseMotion::MOVEMENT, Input::MouseMotionEventType::NOT_APPLICABLE, [&cameraTransform](int, int, int xRel, int yRel) {
        if (!Device::isMouseCapturedWindow(Engine::getMainWindow()))
            return;

        float xOffset = static_cast<float>(xRel) * mouseSensitivity * (static_cast<float>(Engine::getDeltaTicks()) / 1000);
        float yOffset = static_cast<float>(yRel) * mouseSensitivity * (static_cast<float>(Engine::getDeltaTicks()) / 1000);

        float yaw = glm::degrees(cameraTransform.getYaw());
        yaw -= xOffset;

        float pitch = glm::degrees(cameraTransform.getPitch());
        pitch -= yOffset;
        pitch = std::clamp(pitch, -89.9f, 89.9f);

        cameraTransform.setYaw(glm::radians(yaw));
        cameraTransform.setPitch(glm::radians(pitch));
    });
    Input::MouseMotionEvent::create(Input::MouseMotion::SCROLL, Input::MouseMotionEventType::NOT_APPLICABLE, [&cameraTransform](int x, int y, int, int) {
        if (Device::isMouseCapturedWindow(Engine::getMainWindow()))
            cameraTransform.translateWithRotation({
                static_cast<float>(x) * scrollModifier / 40,
                0,
                -static_cast<float>(y) * scrollModifier / 40 // negate for OpenGL
            });
    });
}

int main(int argc, const char* const argv[]) {
    Engine::preInit(argc, argv);
    Resource::addResourceProvider(new FilesystemResourceProvider{"editor"});
    TranslationManager::addTranslationFile("file://i18n/editor");
    TranslationManager::addUniversalFile("file://i18n/editor");

#ifdef CHIRA_USE_DISCORD
    if (auto* discord_enable = ConEntryRegistry::getConVar("discord_enable"); discord_enable && discord_enable->getValue<bool>()) {
        DiscordRPC::init(TR("editor.discord.application_id"));
        DiscordRPC::setLargeImage("main_logo");
        DiscordRPC::setTopButton({"Join Discord", "https://discord.gg/ASgHFkX"});
    }
#endif

#if defined(CHIRA_USE_STEAMWORKS) && defined(DEBUG)
    if (auto* steam_enable = ConEntryRegistry::getConVar("steam_enable"); steam_enable && steam_enable->getValue<bool>()) {
        // Steam API docs say this is bad practice, I say I don't care
        SteamAPI::generateAppIDFile(1728950);
    }
#endif

    Engine::init();

    auto* layer = Device::getWindowLayer(Engine::getMainWindow());
    layer->setBackgroundColor({0.15f});

    auto* scene = layer->addScene();
    Device::addPanelToWindow(Engine::getMainWindow(), new ModelViewerPanel{scene});

    auto* camera = scene->addEntity();
    camera->addComponent<CameraComponent>(CameraComponent::ProjectionMode::PERSPECTIVE);

    auto& cameraTransform = camera->getTransform();
    cameraTransform.translate({-6.f * sqrtf(3.f), 6, 0});
    cameraTransform.setPitch(glm::radians(-30.f));
    cameraTransform.setYaw(glm::radians(270.f));

    auto* grid = scene->addEntity();
    auto& gridMesh = grid->addComponent<MeshDynamicComponent>();
    gridMesh.meshBuilder.setMaterial(CHIRA_GET_MATERIAL("MaterialUntextured", "file://materials/unlit.json"));
    for (int i = -5; i <= 5; i++) {
        gridMesh.meshBuilder.addCube(Vertex{{i, 0, 0}}, {0.025f, 0.025f, 10.025f});
        gridMesh.meshBuilder.addCube(Vertex{{0, 0, i}}, {10.025f, 0.025f, 0.025f});
    }
    gridMesh.meshBuilder.addCube({{2.5f, 0, 0}, {0, 0, 0}, {1, 0, 0}}, {5.f + 0.026f, 0.03f, 0.03f});
    gridMesh.meshBuilder.addCube({{0, 0, 2.5f}, {0, 0, 0}, {0, 0, 1}}, {0.03f, 0.03f, 5.f + 0.026f});
    gridMesh.meshBuilder.addCube({{0, 0,    0}, {0, 0, 0}, {0, 1, 0}}, glm::vec3{0.05f});

    setupKeybinds(cameraTransform);

    Engine::run();
}
