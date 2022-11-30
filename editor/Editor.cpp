#include "Editor.h"

// Disable console window on Windows (MSVC)
// EXPLAIN: what is the issue with having a console window?
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
#include "camera/EditorCamera.h"
#include <ui/IPanel.h>
#include <ui/FramePanel.h>
#include <utility/Dialogs.h>
#include <render/material/MaterialPhong.h>

// Used because file dialogs are broken on macOS
// TODO: Make these prettier.
#include <imfilebrowser.h>

#include <imgui_internal.h>

// All the UI elements in the editor
#include <editorui/EngineView.h>
#include <editorui/ResourceBrowser.h>
#include <editorui/EntityPanel.h>
#include <editorui/CodeView.h>
#include <editorui/ToolsPanel.h>

#ifdef CHIRA_USE_DISCORD
    #include <hook/DiscordRPC.h>
#endif
#ifdef CHIRA_USE_STEAMWORKS
    #include <hook/SteamAPI.h>
#endif

// Need to register phong material!
#include <render/material/MaterialPhong.h>

using namespace chira;

CHIRA_CREATE_LOG(EDITOR);

unsigned int engineviewID = 0;
unsigned int resourcepanelID = 0;
unsigned int codePanelID = 0;

static inline void addResourceFolderSelected() {
    auto folder = Dialogs::openFolder();
    if (folder.empty())
        return; // cancelled

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

MainEditorPanel::MainEditorPanel() : IPanel(TR("ui.window.title"), true) {
    this->flags |= ImGuiWindowFlags_NoBackground |
                    ImGuiWindowFlags_NoInputs   |
                    ImGuiWindowFlags_NoDecoration;
}

// Opens a file dialog used to select a model definition
void MainEditorPanel::addModelSelected() {
    std::string path = FilesystemResourceProvider::getResourceIdentifier(modeldialog.GetSelected().string());
    if (!path.empty())
            this->setLoadedFile(path);
}

void MainEditorPanel::convertToModelTypeSelected(const std::string& extension, const std::string& type) const {
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

void MainEditorPanel::convertToOBJSelected() const {
    this->convertToModelTypeSelected(".obj", "obj");
}

void MainEditorPanel::convertToCMDLSelected() const {
    this->convertToModelTypeSelected(".cmdl", "cmdl");
}

void MainEditorPanel::preRenderContents() {
    modeldialog.SetTitle("Open Resource");
    modeldialog.SetTypeFilters({".mdef"});
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(TRC("ui.menubar.file"))) { // File
            if (ImGui::MenuItem(TRC("ui.menubar.open_model"))) { // Open Model...
                modeldialog.Open();
            }
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
        if (ImGui::BeginMenu(TRC("ui.menubar.window"))) {
            
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    // Model Dialog specific logic
    modeldialog.Display();
    if (modeldialog.HasSelected()) {
        this->addModelSelected();
        modeldialog.ClearSelected();
    }

}

void MainEditorPanel::renderContents() {
    if (auto* framePanel = Engine::getWindow()->getPanel<EngineView>(engineviewID)) {
        framePanel->loadedFile = this->loadedFile;
    }
    ImGui::ShowDemoWindow();
}

void MainEditorPanel::setLoadedFile(const std::string& meshName) {
    if (!Resource::hasResource(meshName)) {
        Dialogs::popupError(TRF("error.modelviewer.resource_is_invalid", meshName));
        return;
    }
    if (auto* framePanel = Engine::getWindow()->getPanel<FramePanel>(engineviewID)) {
        if (framePanel->getFrame()->hasChild(this->meshId)) {
            framePanel->getFrame()->removeChild(this->meshId);
        }
        framePanel->getFrame()->addChild(new Mesh{meshName});
    }
    this->loadedFile = meshName;
}

std::string_view MainEditorPanel::getMeshId() const {
    return this->meshId;
}

static void CreateInitialLayout() {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("ChiraEditDockspace", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("ChiraEditDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    ImGui::DockBuilderRemoveNode(dockspace_id);                            // Clear out existing layout
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
    ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowSize());

    ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
    ImGuiID dock_id_prop = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
    ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, NULL, &dock_main_id);

    ImGui::DockBuilderDockWindow(TRC("ui.resourcebrowser.title"), dock_id_bottom);
    ImGui::DockBuilderDockWindow(TRC("ui.entitypanel.title"), dock_id_prop);
    ImGui::DockBuilderFinish(dockspace_id);
    ImGui::End();
}

static void setup_colors()
{
    auto& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg]               = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.87f, 0.31f, 0.96f, 0.70f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.87f, 0.31f, 0.96f, 0.70f);
    colors[ImGuiCol_Button]                 = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.87f, 0.31f, 0.96f, 0.70f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 0.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 0.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.31f, 0.96f, 0.70f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.87f, 0.31f, 0.96f, 0.70f);

    style.WindowPadding     = ImVec2(4, 4);
    style.FramePadding      = ImVec2(6, 6);
    style.ItemSpacing       = ImVec2(4, 2);

    style.ScrollbarSize     = 12;
    style.ScrollbarRounding = 12;

    style.IndentSpacing     = 12;

    style.WindowBorderSize  = 1;
    style.ChildBorderSize   = 0;
    style.PopupBorderSize   = 0;
    style.FrameBorderSize   = 0;
    style.PopupRounding     = 0;

    style.FrameRounding     = 3;
    style.GrabRounding      = 3;

    style.WindowRounding    = 0;
    style.ChildRounding     = 0;
    style.TabBorderSize     = 0;
    style.TabRounding       = 0;

    if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

int main(int argc, const char* const argv[]) {
    Engine::preInit(argc, argv);
    Resource::addResourceProvider(new FilesystemResourceProvider{"editor"});
    TranslationManager::addTranslationFile("file://i18n/editor");
    TranslationManager::addUniversalFile("file://i18n/editor");

#ifdef CHIRA_USE_DISCORD
    if (auto* discord_enable = ConVarRegistry::getConVar("discord_enable"); discord_enable && discord_enable->getValue<bool>()) {
        DiscordRPC::init(TR("editor.discord.application_id"));
        DiscordRPC::setLargeImage("main_logo");
        DiscordRPC::setTopButton({"Join Discord", "https://discord.gg/ASgHFkX"});
    }
#endif

#if defined(CHIRA_USE_STEAMWORKS) && defined(DEBUG)
    if (auto* steam_enable = ConVarRegistry::getConVar("steam_enable"); steam_enable && steam_enable->getValue<bool>()) {
        // TODO: look into what this bad practice is and replace it with a good practice instead.
        SteamAPI::generateAppIDFile(1728950);
    }
#endif

    Engine::init();

    setup_colors();

        Engine::getWindow()->setBackgroundColor(ColorRGB{0.15f});

    auto mainPanel = new MainEditorPanel();
    Engine::getWindow()->addPanel(mainPanel);

    auto framePanel = new EngineView();
    auto frame = framePanel->getFrame();
    frame->setBackgroundColor(ColorRGB{0.15f});
    engineviewID = Engine::getWindow()->addPanel(framePanel);

    // Engine tool panels
    // The resource browser
    auto resourcePanel = new ResourceBrowser(mainPanel);
    resourcepanelID = Engine::getWindow()->addPanel(resourcePanel);
    // IDEA: having a game config system to define folders and whatnot would be cool
    resourcePanel->loadResourceFolder("editor");

    // Code View
    auto codePanel = new CodePanel();
    codePanelID = Engine::getWindow()->addPanel(codePanel);

    Engine::getWindow()->addPanel(new EntityPanel(frame));
    Engine::getWindow()->addPanel(new ToolsPanel(frame));

    auto camera = new EditorCamera{CameraProjectionMode::PERSPECTIVE, 120.f};
    camera->translate({-6.f * sqrtf(3.f), 6, 0});
    camera->setPitch(30.f);
    camera->setYaw(270.f);
    camera->setName("EDITOR_CAMERA_DO_NOT_REMOVE");
    frame->addChild(camera);
    frame->setCamera(camera);

    //todo: the camera must be set for the window root for keybinds to affect it
    //EXPLAIN: Todo? what is there todo here? It looks like it's already done from the code?
    Engine::getWindow()->setCamera(camera);
    EditorCamera::setupKeybinds();

    // OPTIMIZE: replace this multi cube mesh grid with a single textured plane grid
    // IDEA: the grid texture I made for Chisel could be reused here!
    auto grid = new MeshDynamic{"EDITOR_GRID_DO_NOT_REMOVE"};
    auto gridMesh = grid->getMesh();
    gridMesh->setMaterial(CHIRA_GET_MATERIAL("MaterialUntextured", "file://materials/unlit.json"));
    for (int i = -5; i <= 5; i++) {
        gridMesh->addCube(Vertex{{i, 0, 0}}, {0.025f, 0.025f, 10.025f});
        gridMesh->addCube(Vertex{{0, 0, i}}, {10.025f, 0.025f, 0.025f});
    }
    gridMesh->addCube({{2.5f, 0, 0}, {0, 0, 0}, {1, 0, 0}}, {5.f + 0.026f, 0.03f, 0.03f});
    gridMesh->addCube({{0, 0, 2.5f}, {0, 0, 0}, {0, 0, 1}}, {0.03f, 0.03f, 5.f + 0.026f});
    gridMesh->addCube({{0, 0,    0}, {0, 0, 0}, {0, 1, 0}}, glm::vec3{0.05f});
    frame->addChild(grid);

    Engine::run();

    CreateInitialLayout();
}
