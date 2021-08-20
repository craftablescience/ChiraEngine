#include "console.h"

console::console() {
    this->clearLog();
    this->historyPos = -1;
    this->autoScroll = true;
    this->scrollToBottom = false;

    this->isEnabled = false;
}

console::~console() {
    this->clearLog();
    for (auto& i : this->history) {
        free(i);
    }
}

void console::clearLog() {
    for (auto& item : this->items) {
        free(item);
    }
    this->items.clear();
}

void console::addLog(const std::string& message) {
    this->items.push_back(strdup(message.c_str()));
}

void console::engineLoggingHook(const loggerType type, const std::string& source, const std::string& message) {
    switch (type) {
        case INFO:
            this->addLog(chiraLogger::INFO_PREFIX + "[" + source + "] " + message);
            break;
        case INFO_IMPORTANT:
            this->addLog(chiraLogger::INFO_IMPORTANT_PREFIX + "[" + source + "] " + message);
            break;
        case OUTPUT:
            this->addLog(chiraLogger::OUTPUT_PREFIX + "[" + source + "] " + message);
            break;
        case WARN:
            this->addLog(chiraLogger::WARNING_PREFIX + "[" + source + "] " + message);
            break;
        case ERR:
            this->addLog(chiraLogger::ERROR_PREFIX + "[" + source + "] " + message);
            break;
    }
}

void console::render() {
    console::setTheme();
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Console", &(this->isEnabled))) {
        ImGui::End();
        return;
    }
    ImGui::Checkbox("Autoscroll", &this->autoScroll);
    ImGui::Separator();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
    ImGuiListClipper clipper;
    clipper.Begin(this->items.size());
    while (clipper.Step()) {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
            const char* item = this->items[i];
            ImVec4 color;
            bool has_color = false;
            if (strstr(item, chiraLogger::INFO_IMPORTANT_PREFIX.c_str())) {
                color = ImVec4(0.13f, 0.77f, 0.13f, 1.0f);
                has_color = true;
            } else if (strstr(item, chiraLogger::OUTPUT_PREFIX.c_str())) {
                color = ImVec4(0.3f, 0.3f, 1.0f, 1.0f);
                has_color = true;
            } else if (strstr(item, chiraLogger::WARNING_PREFIX.c_str())) {
                color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);
                has_color = true;
            } else if (strstr(item, chiraLogger::ERROR_PREFIX.c_str())) {
                color = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                has_color = true;
            }
            if (has_color) {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            }
            ImGui::TextUnformatted(item);
            if (has_color) {
                ImGui::PopStyleColor();
            }
        }
    }

    if (this->scrollToBottom || (this->autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
        ImGui::SetScrollHereY(1.0f);
    }
    this->scrollToBottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();

    ImGui::End();
}

void console::setEnabled(bool enabled) {
    this->isEnabled = enabled;
}

bool console::getEnabled() const {
    return this->isEnabled;
}

void console::setTheme() {
    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg]              = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
    colors[ImGuiCol_ChildBg]               = ImVec4(0.33f, 0.33f, 0.33f, 0.80f);
    colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.33f, 0.33f, 0.33f, 0.80f);
    colors[ImGuiCol_PopupBg]               = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_Border]                = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_BorderShadow]          = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_FrameBg]               = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_TitleBg]               = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_TitleBgActive]         = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_CheckMark]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_Button]                = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_ButtonHovered]         = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_ButtonActive]          = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
    colors[ImGuiCol_Separator]             = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_SeparatorActive]       = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_ResizeGrip]            = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameBorderSize   = 1.0f;
    style.WindowRounding    = 4.0f;
    style.ChildRounding     = 0.0f;
    style.FrameRounding     = 0.0f;
    style.PopupRounding     = 0.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding      = 0.0f;
    style.TabRounding       = 0.0f;
}
