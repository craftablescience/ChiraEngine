#pragma once

#include <ui/IPanel.h>

namespace chira {

class ResourceBrowser : public IPanel {
public:
    ResourceBrowser();
    ~ResourceBrowser();
    void preRenderContents() override;
}

}