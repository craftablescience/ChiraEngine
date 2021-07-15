#ifndef BASICGAMEENGINE_CONSOLE_H
#define BASICGAMEENGINE_CONSOLE_H


#include <string>
#include "imgui.h"
#include "../utility/logger.h"

struct console {
public:
    console();
    virtual ~console();

    //static void strtrim(char* s);

    void clearLog();
    void addLog(const std::string& message);
    void engineLoggingHook(loggerType type, const std::string& source, const std::string& message);
    void render();
    //void execCommand(const char* command_line);

    void setEnabled(bool enabled);
    [[nodiscard]] bool getEnabled() const;
private:
    //char inputBuf[256];
    ImVector<char*> items;
    //ImVector<const char*> commands;
    ImVector<char*> history;
    int historyPos;
    bool autoScroll;
    bool scrollToBottom;
    bool isEnabled;
};


#endif //BASICGAMEENGINE_CONSOLE_H
