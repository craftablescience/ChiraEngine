#pragma once

#include "panel.h"

//todo(input): implement raycasting

namespace chira {
    class PanelFrame : public Panel {
    public:
        PanelFrame(const std::string& title_, bool startVisible, ImVec2 windowSize, bool enforceSize = false);
        void renderContents() override;
        const Frame* getFrame() const override;
        Frame* getFrame() override;
        const Root* getRoot() const override;
        Root* getRoot() override;
    protected:
        Frame* frame = nullptr;
        glm::vec<2, int> currentSize;
    };
}
