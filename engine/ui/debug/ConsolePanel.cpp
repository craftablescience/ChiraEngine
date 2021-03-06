#include "ConsolePanel.h"

#include <config/ConEntry.h>
#include <i18n/TranslationManager.h>
#include <utility/String.h>

using namespace chira;

ConsolePanel::ConsolePanel(ImVec2 windowSize) : IPanel(TR("ui.console.title"), false, windowSize) {
    this->loggingId = Logger::addCallback([&](LogType type, std::string_view source, std::string_view message) {
        static const auto* log_print_source = ConVarRegistry::getConVar("log_print_source");
        std::string logSource{};
        if (log_print_source->getValue<bool>()) {
            logSource += "[";
            logSource += source.data();
            logSource += "]";
        }
        switch (type) {
            case LogType::LOG_INFO:
                this->addLog(std::string{Logger::INFO_PREFIX} + logSource + " " + message.data());
                break;
            case LogType::LOG_INFO_IMPORTANT:
                this->addLog(std::string{Logger::INFO_IMPORTANT_PREFIX} + logSource + " " + message.data());
                break;
            case LogType::LOG_OUTPUT:
                this->addLog(std::string{Logger::OUTPUT_PREFIX} + logSource + " " + message.data());
                break;
            case LogType::LOG_WARNING:
                this->addLog(std::string{Logger::WARNING_PREFIX} + logSource + " " + message.data());
                break;
            case LogType::LOG_ERROR:
                this->addLog(std::string{Logger::ERROR_PREFIX} + logSource + " " + message.data());
                break;
        }
    });
    this->autoScroll = true;
    this->font = Resource::getResource<FontResource>(TR("resource.font.console_font_path"));
}

ConsolePanel::~ConsolePanel() {
    Logger::removeCallback(this->loggingId);
}

void ConsolePanel::renderContents() {
    this->setTheme();

    ImGui::Checkbox("Autoscroll", &this->autoScroll);

    ImGui::Separator();

    const float freeSpace = ImGui::GetFrameHeightWithSpacing() + (ImGui::GetStyle().ItemSpacing.y * 2);
    ImGui::BeginChild("ScrollingRegion", {0, -freeSpace}, false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{4, 1});
    ImGuiListClipper clipper;
    clipper.Begin(static_cast<int>(this->items.size()));
    while (clipper.Step()) {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
            ImVec4 color;
            bool has_color = false;
            if (String::startsWith(this->items[i], Logger::INFO_IMPORTANT_PREFIX)) {
                color = ImVec4(0.13f, 0.77f, 0.13f, 1.0f);
                has_color = true;
            } else if (String::startsWith(this->items[i], Logger::OUTPUT_PREFIX)) {
                color = ImVec4(0.3f, 0.3f, 1.0f, 1.0f);
                has_color = true;
            } else if (String::startsWith(this->items[i], Logger::WARNING_PREFIX)) {
                color = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);
                has_color = true;
            } else if (String::startsWith(this->items[i], Logger::ERROR_PREFIX)) {
                color = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                has_color = true;
            }
            if (has_color) {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            }
            ImGui::TextUnformatted(this->items[i].c_str());
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

    ImGui::Separator();

    ImGui::PushItemWidth(-1);
    bool reclaimFocus = false; // only grab keyboard focus on Enter
    char buf[1024] {0};
    if (ImGui::InputText("CommandInput", buf, sizeof(buf), ImGuiInputTextFlags_EnterReturnsTrue)) {
        reclaimFocus = true;
        this->processConsoleMessage(buf);
    }
    ImGui::PopItemWidth();

    ImGui::SetItemDefaultFocus();
    if (this->wasActivatedThisFrame() || reclaimFocus)
        ImGui::SetKeyboardFocusHere(-1);

    this->resetTheme();
}

void ConsolePanel::clear() {
    this->items.clear();
}

void ConsolePanel::addLog(const std::string& message) {
    if (this->items.size() == ConsolePanel::MAX_ITEM_COUNT) {
        this->items.pop_front();
    }
    this->items.push_back(message);
}

void ConsolePanel::setTheme() {
    if (!this->font) {
        this->font = Resource::getResource<FontResource>(TR("resource.font.console_font_path"));
    }
    ImGui::PushFont(this->font->getFont());
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
}

void ConsolePanel::resetTheme() const {
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);
    ImGui::PopFont();
}

void ConsolePanel::processConsoleMessage(std::string_view message) {
    auto entryList = String::split(message.data(), ';');
    for (const auto& entry : entryList) {
        std::vector<std::string> input = String::split(String::strip(entry), ' ');
        if (!input.empty()) {
            // Special case
            if (input[0] == "clear") {
                this->clear();
            } else if (ConCommandRegistry::hasConCommand(input[0])) {
                std::string name{input[0]};
                input.erase(input.begin());
                ConCommandRegistry::getConCommand(name)->fire(input);
            } else if (ConVarRegistry::hasConVar(input[0])) {
                auto* convar = ConVarRegistry::getConVar(input[0]);
                if (input.size() >= 2) {
                    if (convar->hasFlag(CON_FLAG_READONLY)) {
                        Logger::log(LogType::LOG_ERROR, "Console", std::string{"Cannot set value of readonly convar \""} + convar->getName().data() + "\"!");
                        return;
                    }
                    try {
                        switch (convar->getType()) {
                            case ConVarType::BOOLEAN:
                                if (String::toLower(input[1]) == "true") {
                                    convar->setValue(true);
                                } else if (String::toLower(input[1]) == "false") {
                                    convar->setValue(false);
                                } else {
                                    convar->setValue(static_cast<bool>(std::stoi(input[1])));
                                }
                                break;
                            case ConVarType::INTEGER:
                                convar->setValue(std::stoi(input[1]));
                                break;
                            case ConVarType::DOUBLE:
                                convar->setValue(std::stod(input[1]));
                                break;
                            case ConVarType::STRING:
                                convar->setValue(input[1]);
                        }
                    } catch (const std::invalid_argument &) {
                        Logger::log(LogType::LOG_ERROR, "Console", std::string{"Cannot set value of \""} + convar->getName().data() + "\" to \"" + input[1] + "\"");
                        return;
                    }
                }
                std::string logOutput{convar->getName().data()};
                logOutput += ": ";
                logOutput += convar->getTypeAsString();
                logOutput += " = ";
                switch (convar->getType()) {
                    case ConVarType::BOOLEAN:
                        if (convar->getValue<bool>()) {
                            logOutput += "true";
                        } else {
                            logOutput += "false";
                        }
                        break;
                    case ConVarType::INTEGER:
                        logOutput += std::to_string(convar->getValue<int>());
                        break;
                    case ConVarType::DOUBLE:
                        logOutput += std::to_string(convar->getValue<double>());
                        break;
                    case ConVarType::STRING:
                        logOutput += convar->getValue<std::string>();
                }
                Logger::log(LogType::LOG_INFO_IMPORTANT, "Console", logOutput);
            } else {
                Logger::log(LogType::LOG_ERROR, "Console", "Could not find command or variable \"" + input[0] + R"("! Run "con_entries" to view valid entries.)");
            }
        }
    }
}
