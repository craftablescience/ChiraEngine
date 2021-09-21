#include "console.h"
#include "../resource/resourceManager.h"

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
            this->addLog(chira::logger::INFO_PREFIX + "[" + source + "] " + message);
            break;
        case INFO_IMPORTANT:
            this->addLog(chira::logger::INFO_IMPORTANT_PREFIX + "[" + source + "] " + message);
            break;
        case OUTPUT:
            this->addLog(chira::logger::OUTPUT_PREFIX + "[" + source + "] " + message);
            break;
        case WARN:
            this->addLog(chira::logger::WARNING_PREFIX + "[" + source + "] " + message);
            break;
        case ERR:
            this->addLog(chira::logger::ERROR_PREFIX + "[" + source + "] " + message);
            break;
    }
}

void console::render() {
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(TR("ui.console.title").c_str(), &(this->isEnabled))) {
        ImGui::End();
        return;
    }
    console::setTheme();
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
            if (strstr(item, chira::logger::INFO_IMPORTANT_PREFIX.c_str())) {
                color = ImVec4(0.13f, 0.77f, 0.13f, 1.0f);
                has_color = true;
            } else if (strstr(item, chira::logger::OUTPUT_PREFIX.c_str())) {
                color = ImVec4(0.3f, 0.3f, 1.0f, 1.0f);
                has_color = true;
            } else if (strstr(item, chira::logger::WARNING_PREFIX.c_str())) {
                color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);
                has_color = true;
            } else if (strstr(item, chira::logger::ERROR_PREFIX.c_str())) {
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
    ImGui::PopStyleVar();

    if (this->scrollToBottom || (this->autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
        ImGui::SetScrollHereY(1.0f);
    }
    this->scrollToBottom = false;

    ImGui::EndChild();
    console::resetTheme();
    ImGui::End();
}

void console::setEnabled(bool enabled) {
    this->isEnabled = enabled;
}

bool console::getEnabled() const {
    return this->isEnabled;
}

void console::setTheme() {
    if (!this->font) {
        this->font = resourceManager::getResource<fontResource>(TR("resource.font.console_font_path"));
    }
    ImGui::PushFont(this->font->getFont());

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
}

void console::resetTheme() {
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);
    ImGui::PopFont();
}
