#include "Popups.h"

#include <SDL.h>

using namespace chira;

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
