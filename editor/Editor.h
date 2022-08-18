#pragma once

#include <ui/IPanel.h>
#include <imfilebrowser.h>

namespace chira {

class MainEditorPanel : public IPanel {
public:
    MainEditorPanel();
    void addModelSelected();
    void convertToModelTypeSelected(const std::string& extension, const std::string& type) const;
    void convertToOBJSelected() const;
    void convertToCMDLSelected() const;
    void preRenderContents() override;
    void renderContents() override;
    void setLoadedFile(const std::string& meshName);
    [[nodiscard]] std::string_view getMeshId() const;
private:
    std::string loadedFile;
    std::string meshId;
    ImGui::FileBrowser modeldialog;
};

} // namespace chira
