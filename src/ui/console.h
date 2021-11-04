#pragma once

#include <string>
#include "../resource/fontResource.h"
#include "../utility/logger.h"
#include "abstractUiWindowComponent.h"

namespace chira {
    class console : public abstractUiWindowComponent {
    public:
        explicit console(const ImVec2& windowSize = ImVec2(800, 600));
        ~console();
        void clearLog();
        void addLog(const std::string& message);
        void precacheResource() const;
        void engineLoggingHook(loggerType type, const std::string& source, const std::string& message);
        void draw(double delta) override;
        void setTheme();
        void resetTheme() const;
    private:
        fontResource* font = nullptr;
        ImVector<char*> items;
        ImVector<char*> history;
        bool autoScroll;
    };
}
