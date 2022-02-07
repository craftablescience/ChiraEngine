#pragma once

#include <resource/fontResource.h>
#include <utility/logger.h>
#include "panel.h"

namespace chira {
    class Console : public Panel {
    public:
        explicit Console(ImVec2 windowSize = ImVec2{800, 600});
        ~Console() override;
        void renderContents() override;
        void clearLog();
        void addLog(const std::string& message);
        void setTheme();
        void resetTheme() const;
    private:
        SharedPointer<FontResource> font;
        ImVector<char*> items;
        ImVector<char*> history;
        bool autoScroll;
        uuids::uuid loggingId;
    };
}
