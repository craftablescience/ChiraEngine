#pragma once

#include <string>
#include "imgui.h"
#include "../utility/logger.h"

struct console {
public:
    console();
    virtual ~console();
    void clearLog();
    void addLog(const std::string& message);
    void engineLoggingHook(loggerType type, const std::string& source, const std::string& message);
    void render();
    void setEnabled(bool enabled);
    [[nodiscard]] bool getEnabled() const;
    static void setTheme();
private:
    ImVector<char*> items;
    ImVector<char*> history;
    int historyPos;
    bool autoScroll;
    bool scrollToBottom;
    bool isEnabled;
};
