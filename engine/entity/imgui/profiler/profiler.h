#pragma once

#include "../window.h"

namespace chira {
    class Profiler : public Window {
    public:
        explicit Profiler(const ImVec2& windowSize = ImVec2(800, 600));
        void renderContents() override;
    };
}
