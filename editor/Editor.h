#pragma once

#include <ui/IPanel.h>
#include <imfilebrowser.h>
#include <entity/logic/Script.h>

namespace chira {

class MainEditorPanel : public IPanel {
public:
    MainEditorPanel();
    void preRenderContents() override;
    void renderContents() override;
    // void addTool(EditorPlugin *tool);
    // void removeTool(const std::string& name);
    // EditorPlugin *getTool(const std::string& name);
    [[nodiscard]] std::string_view getMeshId() const;

    // std::vector<EditorPlugin*> editorplugins;
};

} // namespace chira
