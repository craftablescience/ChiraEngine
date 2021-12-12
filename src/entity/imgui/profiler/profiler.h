#pragma once

#include "../window.h"

namespace chira {
    class profiler : public window {
    public:
        explicit profiler(const ImVec2& windowSize = ImVec2(800, 600));
        void renderContents() override;
    };
}
