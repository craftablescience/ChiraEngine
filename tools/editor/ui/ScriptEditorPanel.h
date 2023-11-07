#pragma once

#include <TextEditor.h>

#include <ui/Font.h>
#include <ui/IPanel.h>

namespace chira {

class ScriptEditorPanel : public IPanel {
public:
    ScriptEditorPanel();

    void save();
    void saveAs(const std::string& filename);
    void load(const std::string& filename);

    void preRenderContents() override;
    void renderContents() override;

protected:
    TextEditor editor;
    SharedPointer<Font> font;
    std::string path;
    bool unsaved;

private:
    void setPath(const std::string& path_);
};

} // namespace chira
