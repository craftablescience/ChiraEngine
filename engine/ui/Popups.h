#pragma once

#include <string>
#include <string_view>

namespace chira::Dialogs {

// Copied from SDL
enum PopupFlags {
    POPUP_ERROR                 = 0x00000010,
    POPUP_WARNING               = 0x00000020,
    POPUP_INFO                  = 0x00000040,
    POPUP_BUTTONS_LEFT_TO_RIGHT = 0x00000080,
    POPUP_BUTTONS_RIGHT_TO_LEFT = 0x00000100,
};

/// Display a popup window with the specified message.
void popup(std::string_view message, std::string_view title, unsigned int popupFlags, std::string_view ok = "OK");
/// Display a popup info window with the specified message.
void popupInfo(std::string_view message, std::string_view title = "Info");
/// Display a popup warning window with the specified message.
void popupWarning(std::string_view message, std::string_view title = "Warning");
/// Display a popup error window with the specified message.
void popupError(std::string_view message, std::string_view title = "Error");

/// Display a popup window with the specified message, as well as two buttons.
bool popupChoice(std::string_view message, std::string_view title, unsigned int popupFlags, std::string_view ok = "OK", std::string_view cancel = "Cancel");
/// Display a popup info window with the specified message, as well as Ok and Cancel buttons.
/// Returns true if Ok pressed, false if Cancel pressed.
bool popupInfoChoice(std::string_view message, std::string_view title = "Info");
/// Display a popup warning window with the specified message, as well as Ok and Cancel buttons.
/// Returns true if Ok pressed, false if Cancel pressed.
bool popupWarningChoice(std::string_view message, std::string_view title = "Warning");
/// Display a popup error window with the specified message, as well as Ok and Cancel buttons.
/// Returns true if Ok pressed, false if Cancel pressed.
bool popupErrorChoice(std::string_view message, std::string_view title = "Error");

} // namespace chira::Dialogs
