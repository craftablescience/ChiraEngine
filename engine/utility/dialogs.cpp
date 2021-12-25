#include "dialogs.h"

#include <filesystem>
#include <tinyfiledialogs.h>
#include <i18n/translationManager.h>
#include <resource/provider/filesystemResourceProvider.h>

std::string chira::dialogOpenFile(const std::string& pattern, const std::string& description) {
    const char* filter[1] = {pattern.c_str()};
    std::string path;
    auto result = tinyfd_openFileDialog(
            TR("ui.window.select_file").c_str(),
            std::filesystem::current_path().string().c_str(), 1,
            filter, description.empty() ? nullptr : description.c_str(), 0);
    if (result) {
        path = result;
        return path;
    }
    return "";
}

std::string chira::dialogOpenResource(const std::string& pattern, const std::string& description) {
    const char* filter[1] = {pattern.c_str()};
    auto result = tinyfd_openFileDialog(
            TR("ui.window.select_file").c_str(),
            std::filesystem::current_path().string().c_str(), 1,
            filter, description.empty() ? nullptr : description.c_str(), 0);
    if (result)
        return filesystemResourceProvider::getResourcePath(std::string{result});
    return "";
}

std::string chira::dialogInput(const std::string& message, const std::string& title, const std::string& placeholder) {
    return {tinyfd_inputBox(title.c_str(), message.c_str(), placeholder.c_str())};
}

std::string chira::dialogPassword(const std::string& message, const std::string& title) {
    return {tinyfd_inputBox(title.c_str(), message.c_str(), nullptr)};
}

void chira::dialogPopupInfo(const std::string& message, const std::string& title) {
    tinyfd_messageBox(title.c_str(), message.c_str(), "ok", "info", 1);
}

void chira::dialogPopupWarning(const std::string& message, const std::string& title) {
    tinyfd_messageBox(title.c_str(), message.c_str(), "ok", "warning", 1);
}

void chira::dialogPopupError(const std::string& message, const std::string& title) {
    tinyfd_messageBox(title.c_str(), message.c_str(), "ok", "error", 1);
}

bool chira::dialogPopupInfoChoice(const std::string& message, bool useYesNo, const std::string& title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.c_str(), message.c_str(), useYesNo? "yesno" : "okcancel", "info", 1));
}

bool chira::dialogPopupWarningChoice(const std::string& message, bool useYesNo, const std::string& title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.c_str(), message.c_str(), useYesNo? "yesno" : "okcancel", "warning", 1));
}

bool chira::dialogPopupErrorChoice(const std::string& message, bool useYesNo, const std::string& title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.c_str(), message.c_str(), useYesNo? "yesno" : "okcancel", "error", 1));
}

bool chira::dialogPopupQuestion(const std::string& question, bool useYesNo, const std::string& title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.c_str(), question.c_str(), useYesNo? "yesno" : "okcancel", "question", 1));
}
