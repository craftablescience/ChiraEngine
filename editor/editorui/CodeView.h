#pragma once

#include <ui/IPanel.h>
#include <math/Types.h>

namespace chira {

class CodePanel : public IPanel {
public:
    CodePanel();
    ~CodePanel() override;
    void renderContents() override;
    void loadFile(std::string name);
protected:
    glm::vec2i currentSize;
    std::string currentFile = "none";
};

} // namespace chira