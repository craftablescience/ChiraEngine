#pragma once

#include "../IPanel.h"

namespace chira {

class ResourceUsageTrackerPanel : public IPanel {
public:
    explicit ResourceUsageTrackerPanel(ImVec2 windowSize = ImVec2{800, 600});
    void renderContents() override;
};

} // namespace chira
