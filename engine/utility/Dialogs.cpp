#include "Dialogs.h"

#include <filesystem>
#include <tinyfiledialogs.h>
#include <i18n/TranslationManager.h>
#include <resource/provider/FilesystemResourceProvider.h>

using namespace chira;

std::string Dialogs::openFile(std::string_view pattern, std::string_view description) {
    const char* filter[] = {pattern.data()};
    std::string path;
    auto result = tinyfd_openFileDialog(
            TR("ui.window.select_file").c_str(),
            std::filesystem::current_path().string().c_str(), 1,
            filter, description.empty() ? nullptr : description.data(), 0);
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

std::string Dialogs::openResource(std::string_view pattern, std::string_view description) {
    const char* filter[] = {pattern.data()};
    auto result = tinyfd_openFileDialog(
            TR("ui.window.select_file").c_str(),
            std::filesystem::current_path().string().c_str(), 1,
            filter, description.empty() ? nullptr : description.data(), 0);
    if (result)
        return FilesystemResourceProvider::getResourceIdentifier(std::string{result});
    return "";
}

std::string Dialogs::saveFile(std::string_view pattern, std::string_view defaultName, std::string_view description) {
    const char* filter[] = {pattern.data()};
    std::string path;
    auto result = tinyfd_saveFileDialog(
            TR("ui.window.save_file").c_str(),
            (std::filesystem::current_path().string() + defaultName.data()).c_str(), 1,
            filter, description.empty() ? nullptr : description.data());
    if (result) {
        path = result;
        return path;
    }
    return "";
}

std::string Dialogs::input(std::string_view message, std::string_view title, std::string_view placeholder) {
    return {tinyfd_inputBox(title.data(), message.data(), placeholder.data())};
}

std::string Dialogs::password(std::string_view message, std::string_view title) {
    return {tinyfd_inputBox(title.data(), message.data(), nullptr)};
}

void Dialogs::popupInfo(std::string_view message, std::string_view title) {
    tinyfd_messageBox(title.data(), message.data(), "ok", "info", 1);
}

void Dialogs::popupWarning(std::string_view message, std::string_view title) {
    tinyfd_messageBox(title.data(), message.data(), "ok", "warning", 1);
}

void Dialogs::popupError(std::string_view message, std::string_view title) {
    tinyfd_messageBox(title.data(), message.data(), "ok", "error", 1);
}

bool Dialogs::popupInfoChoice(std::string_view message, bool useYesNo, std::string_view title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.data(), message.data(), useYesNo? "yesno" : "okcancel", "info", 1));
}

bool Dialogs::popupWarningChoice(std::string_view message, bool useYesNo, std::string_view title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.data(), message.data(), useYesNo? "yesno" : "okcancel", "warning", 1));
}

bool Dialogs::popupErrorChoice(std::string_view message, bool useYesNo, std::string_view title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.data(), message.data(), useYesNo? "yesno" : "okcancel", "error", 1));
}

bool Dialogs::popupQuestion(std::string_view question, bool useYesNo, std::string_view title) {
    return static_cast<bool>(
            tinyfd_messageBox(title.data(), question.data(), useYesNo? "yesno" : "okcancel", "question", 1));
}
