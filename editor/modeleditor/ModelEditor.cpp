// Model Editor
// Allows for the viewing, editing, and converting of models
// Directly inside the ChiraEngine editor
// 
// Chira Team

#include "ModelEditor.h"

#include <fstream>
#include <i18n/TranslationManager.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <utility/Dialogs.h>
#include <imfilebrowser.h>
#include <render/material/MaterialPhong.h>
#include <entity/root/Frame.h>
#include "ModelScene.h"
#include <entity/model/Mesh.h>

using namespace chira;

CHIRA_CREATE_LOG(MODELEDITOR);

ModelEditor::ModelEditor()
    : IPanel(TRC("ui.modeleditor.title"), true, ImVec2(200.0F, 200.0F), false) {

    this->modelScene = new ModelScene();
}

void ModelEditor::convertToModelTypeSelected(const std::string& extension, const std::string& type) const {
    std::string filepath = Dialogs::saveFile(extension);
    if (filepath.empty())
        return Dialogs::popupError(TR("error.modelviewer.filename_empty"));

    if (!modelScene->getFrame()->hasChild(this->meshId))
        return Dialogs::popupError(TR("error.modelviewer.no_model_present"));

    std::ofstream file{filepath, std::ios::binary};
    std::vector<byte> meshData = modelScene->getFrame()->getChild<Mesh>(this->meshId)->getMeshData(type);
    file.write(reinterpret_cast<const char*>(&meshData[0]), static_cast<std::streamsize>(meshData.size()));
    file.close();
}

void ModelEditor::convertToOBJSelected() const {
    this->convertToModelTypeSelected(".obj", "obj");
}

void ModelEditor::convertToCMDLSelected() const {
    this->convertToModelTypeSelected(".cmdl", "cmdl");
}

void ModelEditor::setLoadedFile(const std::string& meshName) {
    if (!Resource::hasResource(meshName)) {
        Dialogs::popupError(TRF("error.modelviewer.resource_is_invalid", meshName));
        return;
    }
    if (modelScene->getFrame()->hasChild(this->meshId)) {
        modelScene->getFrame()->removeChild(this->meshId);
    }
    modelScene->getFrame()->addChild(new Mesh{meshName});
    modelScene->loadedFile = meshName;
}

std::string_view ModelEditor::getMeshId() const {
    return this->meshId;
}

void ModelEditor::renderContents() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu(TRC("ui.menubar.file"))) {
            ImGui::MenuItem(TRC("ui.modeleditor.open_model"));

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(TRC("ui.modeleditor.convert"))) {
            if (ImGui::MenuItem(TRC("ui.modeleditor.convert_to_obj"))) {
                ModelEditor::convertToOBJSelected();
            }
            if (ImGui::MenuItem(TRC("ui.modeleditor.convert_to_cmdl"))) {
                ModelEditor::convertToCMDLSelected();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    modelScene->render();
}