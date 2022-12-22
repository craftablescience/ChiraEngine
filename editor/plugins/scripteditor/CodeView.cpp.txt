#include "CodeView.h"

#include <fstream>
#include <streambuf>
#include <filesystem>
#include <core/Engine.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <i18n/TranslationManager.h>
#include <ui/IPanel.h>
#include <ui/FramePanel.h>
#include <utility/Dialogs.h>
#include <thirdparty/imguicolortextedit/TextEditor.h>

using namespace chira;

TextEditor editor;

CodePanel::CodePanel()
	: IPanel(TRC("ui.codeview.title"), true, ImVec2(2.0F, 2.0F), false), currentSize(2.0F, 2.0F) {
		this->flags |= ImGuiWindowFlags_MenuBar;
		editor.SetPalette(TextEditor::GetDarkPalette());
		// for testing purposes
		auto lang = TextEditor::LanguageDefinition::CPlusPlus();
		editor.SetLanguageDefinition(lang);
}

CodePanel::~CodePanel() {

}

void CodePanel::loadFile(std::string name) {
	std::ifstream t(name.c_str());
	if (t.good())
	{
		this->currentFile = name;
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		editor.SetText(str);
	}
}

void CodePanel::renderContents() {
	auto cpos = editor.GetCursorPosition();
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{
			}
			if (ImGui::MenuItem("Save"))
			{
				auto textToSave = editor.GetText();
				/// save text....
			}
			if (ImGui::MenuItem("Open"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor.IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				editor.SetReadOnly(ro);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
				editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
				editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
				editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
				editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
				editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
		editor.IsOverwrite() ? "Ovr" : "Ins",
		editor.CanUndo() ? "*" : " ",
		editor.GetLanguageDefinition().mName.c_str(), this->currentFile.c_str());

	editor.Render("TextEditor");
}