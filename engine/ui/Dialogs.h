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

/// Returns an absolute path to the file selected, or an empty string if none selected.
std::string openFile(std::string_view pattern, std::string_view description = "");
/// Returns an absolute path to the folder selected, or an empty string if none selected.
std::string openFolder();
/// Returns a relative path to the resource selected, or an empty string if none selected.
std::string openResource(std::string_view pattern, std::string_view description = "");
/// Return an absolute path to the file to create, or an empty string if Cancel pressed.
std::string saveFile(std::string_view pattern, std::string_view defaultName = "", std::string_view description = "");

/// Display a popup window with an input box.
std::string input(std::string_view message = "", std::string_view title = "Input", std::string_view placeholder = "");
/// Display a popup window with a password box.
std::string password(std::string_view message = "", std::string_view title = "Password");

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
