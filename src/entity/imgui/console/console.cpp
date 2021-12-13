#include "console.h"

#include <resource/resource.h>
#include <i18n/translationManager.h>
#include <fmt/core.h>

using namespace chira;

console::console(const ImVec2& windowSize) : window(TR("ui.console.title"), false, windowSize) {
    this->loggingId = logger::addCallback([=](const loggerType& type, const std::string& source, const std::string& message) {
        console::engineLoggingHook(type, source, message);
    });
    this->clearLog();
    this->autoScroll = true;
}

console::~console() {
    this->clearLog();
    for (auto& i : this->history) {
        free(i);
    }
    logger::removeCallback(this->loggingId);
}

void console::renderContents() {
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
            if (strstr(item, logger::INFO_IMPORTANT_PREFIX.data())) {
                color = ImVec4(0.13f, 0.77f, 0.13f, 1.0f);
                has_color = true;
            } else if (strstr(item, logger::OUTPUT_PREFIX.data())) {
                color = ImVec4(0.3f, 0.3f, 1.0f, 1.0f);
                has_color = true;
            } else if (strstr(item, logger::WARNING_PREFIX.data())) {
                color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);
                has_color = true;
            } else if (strstr(item, logger::ERROR_PREFIX.data())) {
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
    console::resetTheme();
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

void console::precacheResource() {
    this->font = resource::getResource<fontResource>(TR("resource.font.console_font_path"));
}

void console::engineLoggingHook(const loggerType type, const std::string& source, const std::string& message) {
    switch (type) {
        case INFO:
            this->addLog(std::string(logger::INFO_PREFIX) + "[" + source + "] " + message);
            break;
        case INFO_IMPORTANT:
            this->addLog(std::string(logger::INFO_IMPORTANT_PREFIX) + "[" + source + "] " + message);
            break;
        case OUTPUT:
            this->addLog(std::string(logger::OUTPUT_PREFIX) + "[" + source + "] " + message);
            break;
        case WARN:
            this->addLog(std::string(logger::WARNING_PREFIX) + "[" + source + "] " + message);
            break;
        case ERR:
            this->addLog(std::string(logger::ERROR_PREFIX) + "[" + source + "] " + message);
            break;
    }
}

void console::setTheme() {
    if (!this->font) {
        this->font = resource::getResource<fontResource>(TR("resource.font.console_font_path"));
    }
    ImGui::PushFont(this->font->getFont());
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
}

void console::resetTheme() const {
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);
    ImGui::PopFont();
}
