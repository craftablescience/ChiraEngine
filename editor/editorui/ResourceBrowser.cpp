#include "ResourceBrowser.h"

#include <fstream>
#include <core/Engine.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <i18n/TranslationManager.h>
#include <ui/IPanel.h>
#include <ui/FramePanel.h>
#include <render/texture/Texture.h>
#include <Editor.h>
#include "thirdparty/fontawesome/IconsFontAwesome5.h"

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
	this->previousPath = this->currentPath;
	this->currentPath = path;
	// HACK: we should fix trailing '/' breaking getDirectoryContents' ResFolder check instead of doing this
	std::string tochange = FILESYSTEM_ROOT_FOLDER + "/" + this->curResFolder;
	if (!path.empty())
	 tochange+="/" + path;
	auto dirList = FilesystemResourceProvider::getDirectoryContents(tochange, "");
	this->curdirList = dirList;
}

void ResourceBrowser::renderContents() {
	static float padding = 16.0f;
	static float thumbnailSize = 64.0f;
	float cellSize = thumbnailSize + padding;

	// better toolbar
	if (ImGui::Button("<-"))
	{
		// significantly better back button
	}
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(15, 0));
	ImGui::SameLine();

	ImGui::Button("Grid");
	ImGui::SameLine();
	ImGui::Button("List");
	ImGui::SameLine();

	if (ImGui::BeginPopupContextItem("viewsettings"))
	{
		ImGui::SliderFloat(TRC("ui.resourcebrowser.thumbsize"), &thumbnailSize, 16, 512);
		ImGui::SliderFloat(TRC("ui.resourcebrowser.padding"), &padding, 0, 32);
		ImGui::EndPopup();
	}

	if (ImGui::Button(TRC("ui.menubar.view")))
		ImGui::OpenPopup("viewsettings");

	ImGui::SameLine();
	// display the current directory in the menubar
	ImGui::Text(std::string("file://" + this->currentPath).c_str());

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	// don't attempt to display other files
	if (this->curdirList.empty())
		return;

	for ( auto const& [key, val] : this->curdirList ) {
		bool dirchanged = false;
		ImGui::PushID(val.fileName.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		// better setup for file icons
		auto icon = Resource::getResource<Texture>("file://textures/ui/icons/file.json");
		switch(val.fileType) {
		using enum FileType;
			case FILE_SCRIPT: {
				icon = Resource::getResource<Texture>("file://textures/ui/icons/script.json");
				break;
			}
			case FILE_AUDIO: {
				icon = Resource::getResource<Texture>("file://textures/ui/icons/audio.json");
				break;
			}
			case FILE_TEXTURE: {
				icon = Resource::getResource<Texture>("file://textures/ui/icons/picture.json");
				break;
			}
			case FILE_MODEL: {
				icon = Resource::getResource<Texture>("file://textures/ui/icons/cube.json");
				break;
			}
			case FILE_MATERIAL: {
				icon = Resource::getResource<Texture>("file://textures/ui/icons/picture.json");
				break;
			}
			/*case FILE_IMAGE: {
				icon = Resource::getResource<Texture>("file://textures/ui/icons/file.json");
				break;
			}*/
			case FILE_FONT: {
				icon = Resource::getResource<Texture>("file://textures/ui/icons/font.json");
				break;
			}
			case FILE_MESH: {
				icon = Resource::getResource<Texture>("file://textures/ui/icons/cube.json");
				break;
			}
			case FILE_DIRECTORY: {
				icon = Resource::getResource<Texture>("file://textures/ui/icons/folder.json");
				break;
			}
			
		}
		ImGui::ImageButton((void*)icon->getHandle(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem(TRC("ui.copy"), "Ctrl-C", nullptr))
			{
			}
			if (ImGui::MenuItem(TRC("ui.cut"), "Ctrl-X", nullptr))
			{
			}
			if (ImGui::MenuItem(TRC("ui.delete"), "Del", nullptr))
			{
			}
			if (ImGui::MenuItem(TRC("ui.paste"), "Ctrl-V", nullptr))
			{
			}

			ImGui::EndPopup();
		}
		
		if (ImGui::IsItemActivated())
		{
			switch(val.fileType) {
			using enum FileType;
				case FILE_DIRECTORY: {
					this->changeDirectory(this->currentPath + "/" + val.fileName);
					dirchanged = true;
					break;
				}
				case FILE_MESH: {
					this->mainpanel->setLoadedFile("file:/" + this->currentPath + "/" + val.fileName);
					break;
				}
			}
		}
		
		ImGui::PopStyleColor();	

		ImGui::TextWrapped("%s", val.fileName.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
		
		if (dirchanged)
			break;
	}

	ImGui::Columns(1);
}
