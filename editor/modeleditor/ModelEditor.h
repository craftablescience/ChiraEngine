#pragma once

#include <ui/IPanel.h>
#include "ModelScene.h"

namespace chira {

class ModelEditor : public IPanel {
public:
    ModelEditor();
    void addModelSelected();
    void convertToModelTypeSelected(const std::string& extension, const std::string& type) const;
    void convertToOBJSelected() const;
    void convertToCMDLSelected() const;
    void setLoadedFile(const std::string& meshName);

    void renderContents() override;
    std::string_view getMeshId() const;
private:
    std::string loadedFile;
    std::string meshId;
    ModelScene *modelScene = nullptr;
};

} // namespace chira