#include "console.h"

console::console() {
    // todo: this needs to have a Valve GUI theme for nostalgia
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
            this->addLog(abstractLogger::INFO_PREFIX + "[" + source + "] " + message);
            break;
        case INFO_IMPORTANT:
            this->addLog(abstractLogger::INFO_IMPORTANT_PREFIX + "[" + source + "] " + message);
            break;
        case OUTPUT:
            this->addLog(abstractLogger::OUTPUT_PREFIX + "[" + source + "] " + message);
            break;
        case WARNING:
            this->addLog(abstractLogger::WARNING_PREFIX + "[" + source + "] " + message);
            break;
        case ERROR:
            this->addLog(abstractLogger::ERROR_PREFIX + "[" + source + "] " + message);
            break;
    }
}

void console::render() {
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
            if (strstr(item, abstractLogger::INFO_IMPORTANT_PREFIX.c_str())) {
                color = ImVec4(0.13f, 0.55f, 0.13f, 1.0f);
                has_color = true;
            } else if (strstr(item, abstractLogger::OUTPUT_PREFIX.c_str())) {
                color = ImVec4(0.0f, 0.0f, 0.8f, 1.0f);
                has_color = true;
            } else if (strstr(item, abstractLogger::WARNING_PREFIX.c_str())) {
                color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);
                has_color = true;
            } else if (strstr(item, abstractLogger::ERROR_PREFIX.c_str())) {
                color = ImVec4(0.7f, 0.0f, 0.0f, 1.0f);
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
