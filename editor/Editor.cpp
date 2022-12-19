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
#include <ui/IPanel.h>
#include <utility/Dialogs.h>
#include <render/material/MaterialPhong.h>
#include <imfilebrowser.h>
#include <imgui_internal.h>
#include "EditorPlugin.h"
#include <script/AngelScriptVM.h>

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

unsigned int mpid;
bool about_open = false;

[[maybe_unused]]
static ConCommand load_tool{"load_tool", "Attempts to load the given editor plugin.", [](ConCommand::CallbackArgs args) { // NOLINT(cert-err58-cpp)
    for (const auto& name : args) {
        LOG_EDITOR.info("Attempting to load tool '"+name+"'...");
        MainEditorPanel *panel = Engine::getWindow()->getPanel<MainEditorPanel>(mpid);
        // if (panel->getTool(name) != nullptr) {
        //     LOG_EDITOR.error("Tool '"+name+"' is already loaded!");
        // } else {
            panel->addTool(new EditorPlugin(name));
        // }
    }
}};

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

void MainEditorPanel::addTool(EditorPlugin *tool) {
    this->editorplugins.push_back(tool);
}

EditorPlugin *MainEditorPanel::getTool(std::string name) {
    for (EditorPlugin* tool : this->editorplugins) {
        if (tool->getID() == name) {
            return tool;
        }
    }
}

void MainEditorPanel::preRenderContents() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu(TRC("ui.menubar.file"))) { // File
            ImGui::MenuItem(TRC("ui.menubar.new"));
            ImGui::Separator();
            ImGui::MenuItem(TRC("ui.menubar.open"));
            ImGui::Separator();
            ImGui::MenuItem(TRC("ui.menubar.save"));
            ImGui::MenuItem(TRC("ui.menubar.save_as"));
            ImGui::Separator();
            if (ImGui::MenuItem(TRC("ui.menubar.add_resource_folder"))) // Add Resource Folder...
                addResourceFolderSelected();
            ImGui::Separator();
            if (ImGui::MenuItem(TRC("ui.menubar.exit"))) // Exit
                Engine::getWindow()->shouldStopAfterThisFrame();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(TRC("ui.menubar.window"))) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(TRC("ui.menubar.tools"))) {
            ImGui::MenuItem("Tools Manager");
            ImGui::Separator();
            for (EditorPlugin* tool : this->editorplugins) {
                if (ImGui::MenuItem(tool->getID().c_str())) {

                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(TRC("ui.menubar.help"))) {
            ImGui::MenuItem("ChiraEngine Documentation");
            ImGui::MenuItem("ChiraEditor Help");
            ImGui::Separator();
            ImGui::MenuItem("About Tool");
            if (ImGui::MenuItem("About ChiraEngine")) {
                about_open = true;
            }
            ImGui::EndMenu();
        }

        // TODO: Insert current tool here
        ImGui::Text("              ChiraEngine (editor-extended) [` - Open Console]");

        ImGui::EndMainMenuBar();
    }

}

bool tpc_open = false;
ImGuiWindowFlags popup_flags = ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoResize
    |ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoDocking;

std::string tpc[14] = {
    "AngelScript",
    "Dear ImGui - ocornut",
    "Discord RPC - Discord",
    "fmt", "fontawesome",
    "GLAD", "GLFW", "GLM",
    "imgui-filebrowser - AirGuanZ",
    "imguicolortextedit - BalazsJako",
    "JSON - nlohmann", "LibLoader",
    "stb - nothings", "stduuid - mariusbancila",
};

static void tpCreditsPanel() {
    if (ImGui::Begin("Third Party Credits", &tpc_open,
    popup_flags)) {
        for (std::string line : tpc) {
            ImGui::Text(line.c_str());
        }
        if (ImGui::Button("Close")) {
            tpc_open = false;
        }
    }
    ImGui::End();
}

static void aboutPanel() {
    if (ImGui::Begin("About ChiraEngine", &about_open,
    popup_flags)) {
        ImGui::Text("ChiraEngine (editor-extended)");
        ImGui::Text("Created by craftablescience, ashifolfi, and many others");
        ImGui::Text("Editor by ashifolfi");
        if (ImGui::Button("Third Paty Credits")) {
            tpc_open = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) {
            about_open = false;
            tpc_open = false;
        }
    }
    ImGui::End();
}

void MainEditorPanel::renderContents() {
    // about window
    if (about_open)
        aboutPanel();    
    else
        tpc_open = false;
    if (tpc_open)
        tpCreditsPanel();

    for (EditorPlugin* tool : this->editorplugins) {
        tool->doPlugin();
    }
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

static bool imBegin(const std::string name) {
    return ImGui::Begin(name.c_str());
}

static void imText(const std::string label) {
    ImGui::Text(label.c_str());
}

static void imEnd() {
    ImGui::End();
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

    // register these here
    // TODO: Move these not here. Make a specific set of cpp files for registering functions
    AngelScriptVM::registerGlobalFunction(imBegin, "ImGui_Begin");
    AngelScriptVM::registerGlobalFunction(imText, "ImGui_Text");
    AngelScriptVM::registerGlobalFunction(imEnd, "ImGui_End", "void ImGui_End()");

    setup_colors();

    Engine::getWindow()->setBackgroundColor(ColorRGB{0.15f});

    auto mainPanel = new MainEditorPanel();
    mpid = Engine::getWindow()->addPanel(mainPanel);

    Engine::run();
}