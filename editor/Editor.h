#pragma once

#include <ui/IPanel.h>
#include <imfilebrowser.h>

namespace chira {

class MainEditorPanel : public IPanel {
public:
    MainEditorPanel();
    void preRenderContents() override;
    void renderContents() override;
    [[nodiscard]] std::string_view getMeshId() const;
};

} // namespace chira
