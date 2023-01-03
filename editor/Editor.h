#pragma once

#include <ui/IPanel.h>
#include <imfilebrowser.h>
#include <utility/UUIDGenerator.h>

namespace chira {

extern uuids::uuid mainpanelid;

class ModelViewerPanel : public IPanel {
public:
    ModelViewerPanel();
    void addModelSelected();
    void convertToModelTypeSelected(const std::string& extension, const std::string& type) const;
    void convertToOBJSelected() const;
    void convertToCMDLSelected() const;
    void preRenderContents() override;
    void renderContents() override;
    void setLoadedFile(const std::string& meshName);
    [[nodiscard]] std::string_view getMeshId() const;
    bool showGrid = true;
    std::string loadedFile;
private:
    std::string meshId;
    ImGui::FileBrowser modelDialog;
};

}
