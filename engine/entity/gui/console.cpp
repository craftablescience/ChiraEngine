#include "console.h"

#include <cstring>
#include <resource/resource.h>
#include <i18n/translationManager.h>

using namespace chira;

Console::Console(ImVec2 windowSize) : Panel(TR("ui.console.title"), false, windowSize) {
    this->loggingId = Logger::addCallback([&](LogType type, const std::string& source, const std::string& message) {
        switch (type) {
            case LogType::INFO:
                this->addLog(std::string{LOGGER_INFO_PREFIX} + "[" + source + "] " + message);
                break;
            case LogType::INFO_IMPORTANT:
                this->addLog(std::string{LOGGER_INFO_IMPORTANT_PREFIX} + "[" + source + "] " + message);
                break;
            case LogType::OUTPUT:
                this->addLog(std::string{LOGGER_OUTPUT_PREFIX} + "[" + source + "] " + message);
                break;
            case LogType::WARNING:
                this->addLog(std::string{LOGGER_WARNING_PREFIX} + "[" + source + "] " + message);
                break;
            case LogType::ERROR:
                this->addLog(std::string{LOGGER_ERROR_PREFIX} + "[" + source + "] " + message);
                break;
        }
    });
    this->clearLog();
    this->autoScroll = true;
    this->font = Resource::getResource<FontResource>(TR("resource.font.console_font_path"));
}

Console::~Console() {
    this->clearLog();
    for (auto& i : this->history) {
        free(i);
    }
    Logger::removeCallback(this->loggingId);
}

void Console::renderContents() {
    Console::setTheme();
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
            if (strstr(item, LOGGER_INFO_IMPORTANT_PREFIX.data())) {
                color = ImVec4(0.13f, 0.77f, 0.13f, 1.0f);
                has_color = true;
            } else if (strstr(item, LOGGER_OUTPUT_PREFIX.data())) {
                color = ImVec4(0.3f, 0.3f, 1.0f, 1.0f);
                has_color = true;
            } else if (strstr(item, LOGGER_WARNING_PREFIX.data())) {
                color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);
                has_color = true;
            } else if (strstr(item, LOGGER_ERROR_PREFIX.data())) {
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

    // Subtracting 2 from ImGui::GetScrollMaxY() should give a little leeway
    // when scrolling back down to start autoscrolling again
    if (this->autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 2) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();
    Console::resetTheme();
}

void Console::clearLog() {
    for (auto& item : this->items) {
        free(item);
    }
    this->items.clear();
}

void Console::addLog(const std::string& message) {
    this->items.push_back(strdup(message.c_str()));
}

void Console::setTheme() {
    if (!this->font) {
        this->font = Resource::getResource<FontResource>(TR("resource.font.console_font_path"));
    }
    ImGui::PushFont(this->font->getFont());
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
}

void Console::resetTheme() const {
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);
    ImGui::PopFont();
}
