#pragma once

#include "../../resource/fontResource.h"
#include "../../utility/logger.h"
#include "../window.h"

namespace chira {
    class console : public window {
    public:
        explicit console(const ImVec2& windowSize = ImVec2(800, 600), const std::string& name_ = "Console");
        ~console() override;
        void renderContents() override;
        void clearLog();
        void addLog(const std::string& message);
        static void precacheResource() ;
        void engineLoggingHook(loggerType type, const std::string& source, const std::string& message);
        void setTheme();
        void resetTheme() const;
    private:
        std::shared_ptr<fontResource> font = nullptr;
        ImVector<char*> items;
        ImVector<char*> history;
        bool autoScroll;
    };
}
