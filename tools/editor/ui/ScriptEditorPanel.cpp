#include "ScriptEditorPanel.h"

#include <fstream>
#include <imfilebrowser.h>

#include <i18n/TranslationManager.h>

using namespace chira;

ScriptEditorPanel::ScriptEditorPanel()
        : IPanel("Script Editor", true)
        , unsaved(false) {
    this->flags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar;

    this->setPath("");
    this->editor.SetTabSize(4);

    this->font = Resource::getResource<Font>(TR("resource.font.console"));
}

void ScriptEditorPanel::save() {
	if (this->path.empty()) {
		return;
	}
    this->unsaved = false;

    auto text = this->editor.GetText();
    // get rid of terminating newline
    text = text.substr(0, text.length() - 1);

    std::ofstream{this->path, std::ios::out | std::ios::trunc} << text;
}

void ScriptEditorPanel::saveAs(const std::string& filename) {
    this->setPath(filename);
    this->save();
}

void ScriptEditorPanel::load(const std::string& filename) {
    this->setPath(filename);
    std::ifstream fileData{this->path};
    if (fileData.good()) {
        this->editor.SetText({(std::istreambuf_iterator<char>(fileData)), std::istreambuf_iterator<char>()});
        this->unsaved = false;
    }
}

void ScriptEditorPanel::preRenderContents() {
    if (this->unsaved) {
        this->flags |= ImGuiWindowFlags_UnsavedDocument;
    } else {
        this->flags &= ~ImGuiWindowFlags_UnsavedDocument;
    }
}

void ScriptEditorPanel::renderContents() {
    static ImGui::FileBrowser filePicker{ImGuiFileBrowserFlags_CloseOnEsc};
    static ImGui::FileBrowser fileMaker{ImGuiFileBrowserFlags_CloseOnEsc | ImGuiFileBrowserFlags_EnterNewFilename};

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save", nullptr, nullptr, !this->path.empty())) {
                this->save();
            }
            if (ImGui::MenuItem("Save as...")) {
                fileMaker.Open();
            }
            if (ImGui::MenuItem("Open...")) {
                filePicker.Open();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            bool ro = this->editor.IsReadOnly();

            if (ImGui::MenuItem("Undo", "Alt-Backspace", nullptr, !ro && this->editor.CanUndo())) {
                this->editor.Undo();
            }
            if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && this->editor.CanRedo())) {
                this->editor.Redo();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, this->editor.HasSelection())) {
                this->editor.Copy();
            }
            if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && this->editor.HasSelection())) {
                this->editor.Cut();
            }
            if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && this->editor.HasSelection())) {
                this->editor.Delete();
            }
            if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText())) {
                this->editor.Paste();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Select all")) {
                this->editor.SetSelection(TextEditor::Coordinates(),
                                          TextEditor::Coordinates(this->editor.GetTotalLines(), 0));
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Make read-only", nullptr, &ro)) {
                this->editor.SetReadOnly(ro);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Theme")) {
            if (ImGui::MenuItem("Light")) {
                this->editor.SetPalette(TextEditor::GetLightPalette());
            }
            if (ImGui::MenuItem("Dark")) {
                this->editor.SetPalette(TextEditor::GetDarkPalette());
            }
            if (ImGui::MenuItem("Retro Blue")) {
                this->editor.SetPalette(TextEditor::GetRetroBluePalette());
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::PushFont(this->font->getFont());

    const auto cursorPos = this->editor.GetCursorPosition();
    ImGui::Text("%6d/%-6d %6d lines | %s | %s | %s",
                cursorPos.mLine + 1, cursorPos.mColumn + 1, this->editor.GetTotalLines(),
                this->editor.GetLanguageDefinition().mName.c_str(),
                this->editor.IsOverwrite() ? "OVERWRITE" : "INSERT",
                this->path.c_str());

    const auto subtitle = this->title + "##Editor";
    this->editor.Render(subtitle.c_str());

    if (this->editor.IsTextChanged()) {
        this->unsaved = true;
    }

    ImGui::PopFont();

    // Show file maker (save as)
    fileMaker.Display();
    if (fileMaker.HasSelected()) {
        if (auto filepath = fileMaker.GetSelected().string(); !filepath.empty()) {
            this->saveAs(filepath);
        }
        fileMaker.ClearSelected();
    }

    // Show file picker (open)
    filePicker.Display();
    if (filePicker.HasSelected()) {
        if (auto filepath = filePicker.GetSelected().string(); !filepath.empty()) {
            this->load(filepath);
        }
        filePicker.ClearSelected();
    }
}

void ScriptEditorPanel::setPath(const std::string& path_) {
    // todo: use String::getFileExtension, which is unmerged atm
    this->path = path_;
    if (this->path.ends_with(".as")) {
        this->editor.SetLanguageDefinition(TextEditor::LanguageDefinition::AngelScript());
    } else if (this->path.ends_with(".c")) {
        this->editor.SetLanguageDefinition(TextEditor::LanguageDefinition::C());
    } else if (this->path.ends_with(".cpp") ||
               this->path.ends_with(".h")   ||
               this->path.ends_with(".cxx") ||
               this->path.ends_with(".hxx") ||
               this->path.ends_with(".hpp") ||
               this->path.ends_with(".ixx")) {
        this->editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
    } else if (this->path.ends_with(".glsl") ||
               this->path.ends_with(".vert") ||
               this->path.ends_with(".vsh")  ||
               this->path.ends_with(".frag") ||
               this->path.ends_with(".fsh")  ||
               this->path.ends_with(".geom") ||
               this->path.ends_with(".gsh")  ||
               this->path.ends_with(".comp") ||
               this->path.ends_with(".csh")) {
        this->editor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
    } else if (this->path.ends_with(".hlsl")) {
        this->editor.SetLanguageDefinition(TextEditor::LanguageDefinition::HLSL());
    } else if (this->path.ends_with(".lua")) {
        this->editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
    } else {
        this->editor.SetLanguageDefinition(TextEditor::LanguageDefinition{});
    }
}
