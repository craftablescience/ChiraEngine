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

    if (this->scrollToBottom || (this->autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
        ImGui::SetScrollHereY(1.0f);
    }
    this->scrollToBottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();

    ImGui::End();
    console::resetTheme();
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

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.46f, 0.46f, 0.46f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.33f, 0.33f, 0.33f, 0.80f));
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.33f, 0.33f, 0.33f, 0.80f));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.13f, 0.13f, 0.13f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.25f, 0.25f, 0.25f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.46f, 0.46f, 0.46f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_SeparatorHovered, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_SeparatorActive, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ResizeGrip, ImVec4(0.40f, 0.40f, 0.40f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, ImVec4(0.40f, 0.40f, 0.40f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, ImVec4(0.40f, 0.40f, 0.40f, 1.00f));

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
}

void console::resetTheme() {
    ImGui::PopStyleVar(8);
    ImGui::PopStyleColor(26);
    ImGui::PopFont();
}
