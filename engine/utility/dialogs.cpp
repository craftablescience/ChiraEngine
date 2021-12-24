#include "dialogs.h"

#include <filesystem>
#include <tinyfiledialogs.h>
#include <i18n/translationManager.h>
#include <resource/provider/filesystemResourceProvider.h>

using namespace chira;

std::string dialogs::openFile(const std::string& pattern, const std::string& description) {
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

std::string dialogs::openResource(const std::string& pattern, const std::string& description) {
    const char* filter[1] = {pattern.c_str()};
    auto result = tinyfd_openFileDialog(
            TR("ui.window.select_file").c_str(),
            std::filesystem::current_path().string().c_str(), 1,
            filter, description.empty() ? nullptr : description.c_str(), 0);
    if (result)
        return filesystemResourceProvider::getResourcePath(std::string{result});
    return "";
}

void dialogs::popupInfo(const std::string& message) {
    tinyfd_messageBox("Info", message.c_str(), "ok", "info", 1);
}

void dialogs::popupWarning(const std::string& message) {
    tinyfd_messageBox("Warning", message.c_str(), "ok", "warning", 1);
}

void dialogs::popupError(const std::string& message) {
    tinyfd_messageBox("Error", message.c_str(), "ok", "error", 1);
}

bool dialogs::popupQuestion(const std::string& question, bool useYesNo, const std::string& title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.c_str(), question.c_str(), useYesNo? "yesno" : "okcancel", "question", 1));
}
