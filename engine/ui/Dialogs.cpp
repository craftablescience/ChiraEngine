#include "Dialogs.h"

#include <filesystem>
#include <SDL.h>
#include <tinyfiledialogs.h>
#include <i18n/TranslationManager.h>
#include <resource/provider/FilesystemResourceProvider.h>

// todo: set main window as parent of SDL dialogs
// todo: convert the rest of these to SDL and/or imgui

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

void Dialogs::popup(std::string_view message, std::string_view title, unsigned int popupFlags, std::string_view ok) {
    SDL_MessageBoxButtonData buttons[] {
        {
            .flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
            .buttonid = 0,
            .text = ok.data(),
        },
    };
    SDL_MessageBoxData data{
        .flags = popupFlags,
        .window = nullptr,
        .title = title.data(),
        .message = message.data(),
        .numbuttons = 1,
        .buttons = buttons,
        .colorScheme = nullptr,
    };
    int buttonID;
    SDL_ShowMessageBox(&data, &buttonID);
}

void Dialogs::popupInfo(std::string_view message, std::string_view title) {
    Dialogs::popup(message, title, POPUP_INFO);
}

void Dialogs::popupWarning(std::string_view message, std::string_view title) {
    Dialogs::popup(message, title, POPUP_WARNING);
}

void Dialogs::popupError(std::string_view message, std::string_view title) {
    Dialogs::popup(message, title, POPUP_ERROR);
}

bool Dialogs::popupChoice(std::string_view message, std::string_view title, unsigned int popupFlags, std::string_view ok, std::string_view cancel) {
    SDL_MessageBoxButtonData buttons[] {
            {
                .flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
                .buttonid = 1,
                .text = ok.data(),
            },
            {
                .flags = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,
                .buttonid = 0,
                .text = cancel.data(),
            }
    };
    SDL_MessageBoxData data{
            .flags = popupFlags,
            .window = nullptr,
            .title = title.data(),
            .message = message.data(),
            .numbuttons = 2,
            .buttons = buttons,
            .colorScheme = nullptr,
    };
    int buttonID;
    SDL_ShowMessageBox(&data, &buttonID);
    return buttonID;
}

bool Dialogs::popupInfoChoice(std::string_view message, std::string_view title) {
    return Dialogs::popupChoice(message, title, POPUP_INFO);
}

bool Dialogs::popupWarningChoice(std::string_view message, std::string_view title) {
    return Dialogs::popupChoice(message, title, POPUP_WARNING);
}

bool Dialogs::popupErrorChoice(std::string_view message, std::string_view title) {
    return Dialogs::popupChoice(message, title, POPUP_ERROR);
}
