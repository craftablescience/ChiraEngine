#include "Dialogs.h"

#include <filesystem>
#include <tinyfiledialogs.h>
#include <i18n/TranslationManager.h>
#include <resource/provider/FilesystemResourceProvider.h>

using namespace chira;

std::string Dialogs::openFile(const std::string& pattern, const std::string& description) {
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

std::string Dialogs::openFolder() {
    std::string path;
    auto result = tinyfd_selectFolderDialog(TR("ui.window.select_file").c_str(), std::filesystem::current_path().string().c_str());
    if (result) {
        path = result;
        return path;
    }
    return "";
}

std::string Dialogs::openResource(const std::string& pattern, const std::string& description) {
    const char* filter[1] = {pattern.c_str()};
    auto result = tinyfd_openFileDialog(
            TR("ui.window.select_file").c_str(),
            std::filesystem::current_path().string().c_str(), 1,
            filter, description.empty() ? nullptr : description.c_str(), 0);
    if (result)
        return FilesystemResourceProvider::getResourceIdentifier(std::string{result});
    return "";
}

std::string Dialogs::saveFile(const std::string& pattern, const std::string& defaultName, const std::string& description) {
    const char* filter[1] = {pattern.c_str()};
    std::string path;
    auto result = tinyfd_saveFileDialog(
            TR("ui.window.save_file").c_str(),
            (std::filesystem::current_path().string() + defaultName).c_str(), 1,
            filter, description.empty() ? nullptr : description.c_str());
    if (result) {
        path = result;
        return path;
    }
    return "";
}

std::string Dialogs::input(const std::string& message, const std::string& title, const std::string& placeholder) {
    return {tinyfd_inputBox(title.c_str(), message.c_str(), placeholder.c_str())};
}

std::string Dialogs::password(const std::string& message, const std::string& title) {
    return {tinyfd_inputBox(title.c_str(), message.c_str(), nullptr)};
}

void Dialogs::popupInfo(const std::string& message, const std::string& title) {
    tinyfd_messageBox(title.c_str(), message.c_str(), "ok", "info", 1);
}

void Dialogs::popupWarning(const std::string& message, const std::string& title) {
    tinyfd_messageBox(title.c_str(), message.c_str(), "ok", "warning", 1);
}

void Dialogs::popupError(const std::string& message, const std::string& title) {
    tinyfd_messageBox(title.c_str(), message.c_str(), "ok", "error", 1);
}

bool Dialogs::popupInfoChoice(const std::string& message, bool useYesNo, const std::string& title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.c_str(), message.c_str(), useYesNo? "yesno" : "okcancel", "info", 1));
}

bool Dialogs::popupWarningChoice(const std::string& message, bool useYesNo, const std::string& title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.c_str(), message.c_str(), useYesNo? "yesno" : "okcancel", "warning", 1));
}

bool Dialogs::popupErrorChoice(const std::string& message, bool useYesNo, const std::string& title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.c_str(), message.c_str(), useYesNo? "yesno" : "okcancel", "error", 1));
}

bool Dialogs::popupQuestion(const std::string& question, bool useYesNo, const std::string& title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.c_str(), question.c_str(), useYesNo? "yesno" : "okcancel", "question", 1));
}
