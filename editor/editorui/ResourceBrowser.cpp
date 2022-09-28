#include "ResourceBrowser.h"

#include <fstream>
#include <core/Engine.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <i18n/TranslationManager.h>
#include <ui/IPanel.h>
#include <ui/FramePanel.h>
#include <render/texture/Texture.h>
#include <Editor.h>

using namespace chira;

CHIRA_CREATE_LOG(RESB);

ResourceBrowser::ResourceBrowser(MainEditorPanel* frame)
	: IPanel(TRC("ui.resourcebrowser.title"), true, ImVec2(2.0F, 2.0F), false), currentSize(2.0F, 2.0F) {
		this->mainpanel = frame;
		this->flags |= ImGuiWindowFlags_MenuBar;
}

void ResourceBrowser::loadResourceFolder(std::string resourceFolder) {
	// ready the variables to open a project's resource folder

	// make sure the directory exists first
	auto resourceFolderPath = FilesystemResourceProvider::getResourceFolderPath(FILESYSTEM_ROOT_FOLDER + "/" + resourceFolder);
	if (resourceFolderPath.empty()) {
		LOG_RESB.error("Attempted to load resource folder " + resourceFolder + " but it doesn't exist!");
		return;
	}
	// we got here so it does.
	// set the curResFolder to our loaded resource folder.
	this->curResFolder = resourceFolder;
	// load the contents of the root folder
	this->curdirList = FilesystemResourceProvider::getDirectoryContents(FILESYSTEM_ROOT_FOLDER + "/" + this->curResFolder, "");

	LOG_RESB.info("Loaded resource folder " + resourceFolder);
}

void ResourceBrowser::changeDirectory(std::string path) {
	LOG_RESB.info("New Path is " + path);
	LOG_RESB.info("Current Path is " + this->currentPath);
	this->previousPath = this->currentPath;
	this->currentPath = path;
  auto dirList = FilesystemResourceProvider::getDirectoryContents(FILESYSTEM_ROOT_FOLDER + "/" + this->curResFolder + "/" + path, "");
	this->curdirList = dirList;
}

void ResourceBrowser::renderContents() {
	static float padding = 16.0f;
	static float thumbnailSize = 64.0f;
	float cellSize = thumbnailSize + padding;
	bool breakout = false;

	if (ImGui::BeginMenuBar())
	{
		// hackjob toolbar in the menubar
		if (ImGui::Button("Back"))
		{
			this->changeDirectory(this->previousPath);
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr))
			{
			}
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr))
			{
			}
			if (ImGui::MenuItem("Delete", "Del", nullptr))
			{
			}
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
			ImGui::SliderFloat("Padding", &padding, 0, 32);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);


	// don't attempt to display other files
	if (this->curdirList.empty())
		return;
	for ( auto const& [key, val] : this->curdirList ) {
		ImGui::PushID(val.fileName.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		switch(val.fileType) {
		using enum FileType;
			case FILE_GENERIC: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/file.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				break;
			}
			case FILE_SCRIPT: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/script.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				break;
			}
			case FILE_AUDIO: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/audio.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				break;
			}
			case FILE_TEXTURE: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/picture.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				break;
			}
			case FILE_MODEL: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/cube.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				break;
			}
			case FILE_MATERIAL: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/picture.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				break;
			}
			case FILE_IMAGE: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/file.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				break;
			}
			case FILE_FONT: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/font.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				break;
			}
			case FILE_MESH: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/cube.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				break;
			}
			case FILE_DIRECTORY: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/folder.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				if (ImGui::IsItemActivated())
				{
					this->changeDirectory(this->currentPath + "/" + val.fileName);
					breakout = true;
					goto swapbreak;
				}
				break;
			}
			default: {
				auto icon = Resource::getResource<Texture>("file://textures/ui/icons/file.json");
				ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				break;
			}
		}
		
swapbreak:
		ImGui::PopStyleColor();
		if (breakout == true)
		{
			ImGui::PopID();
			return;
		}

		ImGui::TextWrapped(val.fileName.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	ImGui::Columns(1);
}
