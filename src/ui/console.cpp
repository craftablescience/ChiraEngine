#include "console.h"

console::console() {//: inputBuf() {
    // todo: this needs to have a Valve GUI theme for nostalgia

    this->clearLog();
    this->historyPos = -1;
    /*
    this->commands.push_back("HELP");
    this->commands.push_back("HISTORY");
    this->commands.push_back("CLEAR");
    this->commands.push_back("CLASSIFY");
    */
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

/*
void console::strtrim(char* s) {
    char* str_end = s + strlen(s);
    while (str_end > s && str_end[-1] == ' ') str_end--;
    *str_end = 0;
}
*/

void console::clearLog() {
    for (auto& item : this->items) {
        free(item);
    }
    this->items.clear();
}

void console::addLog(const std::string& message) {
    items.push_back(strdup(message.c_str()));
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
    /*
    if (ImGui::Button("Clear")) {
        clearLog();
    }
    ImGui::SameLine();
    */
    bool copy_to_clipboard = ImGui::Button("Copy");
    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &this->autoScroll);

    ImGui::Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginPopupContextWindow()) {
        if (ImGui::Selectable("Clear")) clearLog();
        ImGui::EndPopup();
    }

    // Display every line as a separate entry so we can change their color or add custom widgets.
    // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
    // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
    // to only process visible items. The clipper will automatically measure the height of your first item and then
    // "seek" to display only items in the visible area.
    // To use the clipper we can replace your standard loop:
    //      for (int i = 0; i < items.Size; i++)
    //   With:
    //      ImGuiListClipper clipper;
    //      clipper.Begin(items.Size);
    //      while (clipper.Step())
    //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
    // - That your items are evenly spaced (same height)
    // - That you have cheap random access to your elements (you can access them given their index,
    //   without processing all the ones before)
    // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
    // We would need random-access on the post-filtered list.
    // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
    // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
    // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
    // to improve this example code!
    // If your items are of variable height:
    // - Split them into same height items would be simpler and facilitate random-seeking into your list.
    // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
    if (copy_to_clipboard)
        ImGui::LogToClipboard();
    for (int i = 0; i < items.Size; i++) {
        const char* item = items[i];

        // Normally you would store more information in your item than just a string.
        // (e.g. make items[] an array of structure, store color/type etc.)
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
        if (has_color)
            ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(item);
        if (has_color)
            ImGui::PopStyleColor();
    }
    if (copy_to_clipboard)
        ImGui::LogFinish();

    if (scrollToBottom || (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    scrollToBottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    //ImGui::Separator();

    // Command-line
    /*
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText(" | Input", inputBuf, IM_ARRAYSIZE(inputBuf), input_text_flags,
                         [](ImGuiInputTextCallbackData* data) {
        auto* c = (console*) data->UserData;
        switch (data->EventFlag) {
            case ImGuiInputTextFlags_CallbackCompletion: {
                // Example of TEXT COMPLETION
                // Locate beginning of current word

                const char* word_end = data->Buf + data->CursorPos;
                const char* word_start = word_end;
                while (word_start > data->Buf) {
                    const char c = word_start[-1];
                    if (c == ' ' || c == '\t' || c == ',' || c == ';')
                        break;
                    word_start--;
                }

                // Build a list of candidates
                ImVector<const char*> candidates;
                for (auto& command : c->commands) {
                    if (strnicmp(command, word_start, (int) (word_end - word_start)) == 0) {
                        candidates.push_back(command);
                    }
                }
                if (candidates.empty()) {
                    // No match
                    c->addLog("No match for \"" + std::string(word_start).substr(0, (int) (word_end - word_start)) + "\"!\n");
                } else if (candidates.size() == 1) {
                    // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0]);
                    data->InsertChars(data->CursorPos, " ");
                } else {
                    // Multiple matches. Complete as much as we can..
                    // So inputting "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                    int match_len = (int)(word_end - word_start);
                    for (;;) {
                        int cc = 0;
                        bool all_candidates_matches = true;
                        for (int i = 0; i < candidates.size() && all_candidates_matches; i++) {
                            if (i == 0) {
                                cc = toupper(candidates[i][match_len]);
                            } else if (cc == 0 || cc != toupper(candidates[i][match_len])) {
                                all_candidates_matches = false;
                            }
                        }
                        if (!all_candidates_matches) {
                            break;
                        }
                        match_len++;
                    }

                    if (match_len > 0) {
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                        data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                    }

                    // List matches
                    c->addLog("Possible matches:\n");
                    for (auto& candidate : candidates) {
                        c->addLog("- " + std::string(candidate) + "\n");
                    }
                }
                break;
            }
            case ImGuiInputTextFlags_CallbackHistory: {
                // Example of HISTORY
                const int prev_history_pos = c->historyPos;
                if (data->EventKey == ImGuiKey_UpArrow) {
                    if (c->historyPos == -1)
                        c->historyPos = c->history.size() - 1;
                    else if (c->historyPos > 0)
                        c->historyPos--;
                } else if (data->EventKey == ImGuiKey_DownArrow) {
                    if (c->historyPos != -1)
                        if (++c->historyPos >= c->history.size())
                            c->historyPos = -1;
                }
                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != c->historyPos) {
                    const char* history_str = (c->historyPos >= 0) ? c->history[c->historyPos] : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
        }
        return 0;
    }, (void*)this)) {
        char* s = inputBuf;
        strtrim(s);
        if (s[0]) {
            execCommand(s);
        }
        strcpy(s, "");
        reclaim_focus = true;
    }
    */
    ImGui::End();
}

/*
void console::execCommand(const char* command_line) {
    this->addLog(command_line);

    // Insert into history. First find match and delete it, so it can be pushed to the back.
    // This isn't trying to be smart or optimal
    this->historyPos = -1;
    for (int i = this->history.size() - 1; i >= 0; i--) {
        if (stricmp(this->history[i], command_line) == 0) {
            free(this->history[i]);
            this->history.erase(this->history.begin() + i);
            break;
        }
    }
    this->history.push_back(strdup(command_line));

    // Process command
    if (stricmp(command_line, "CLEAR") == 0) {
        clearLog();
    } else if (stricmp(command_line, "HELP") == 0) {
        addLog("commands:");
        for (auto& command : this->commands) {
            addLog("- " + std::string(command));
        }
    } else if (stricmp(command_line, "HISTORY") == 0) {
        int first = this->history.size() - 10;
        for (int i = first > 0 ? first : 0; i < this->history.size(); i++) {
            addLog(std::to_string(i) + ": " + std::string(history[i]) + "\n");
        }
    } else {
        addLog("Unknown command: \"" + std::string(command_line) + "\"\n");
    }
    // On command input, we scroll to bottom even if autoScroll==false
    this->scrollToBottom = true;
}
*/

void console::setEnabled(bool enabled) {
    this->isEnabled = enabled;
}

bool console::getEnabled() const {
    return this->isEnabled;
}
