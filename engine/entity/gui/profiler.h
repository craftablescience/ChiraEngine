#pragma once

#include "panel.h"

namespace chira {
    class Profiler : public Panel {
    public:
        explicit Profiler(ImVec2 windowSize = ImVec2{800, 600});
        void renderContents() override;
    };
}
