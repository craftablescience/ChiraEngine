#ifndef BASICGAMEENGINE_CONSOLE_H
#define BASICGAMEENGINE_CONSOLE_H


#include "imgui.h"

struct console {
public:
    console();
    virtual ~console();

    static int Stricmp(const char* s1, const char* s2);
    static int Strnicmp(const char* s1, const char* s2, int n);
    static char* Strdup(const char* s);
    static void Strtrim(char* s);

    void ClearLog();
    void AddLog(const char* fmt, ...) IM_FMTARGS(2);
    void Render();
    void ExecCommand(const char* command_line);

    void setEnabled(bool enabled);
    [[nodiscard]] bool getEnabled() const;

    // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
    int TextEditCallback(ImGuiInputTextCallbackData* data);
private:
    char InputBuf[256];
    ImVector<char*> Items;
    ImVector<const char*> Commands;
    ImVector<char*> History;
    int HistoryPos;
    ImGuiTextFilter Filter;
    bool AutoScroll;
    bool ScrollToBottom;
    bool isEnabled;
};


#endif //BASICGAMEENGINE_CONSOLE_H
