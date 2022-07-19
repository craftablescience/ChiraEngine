#pragma once

#include <string>
#include <string_view>

namespace chira::Dialogs {

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

/// Display a popup info window with the specified message.
void popupInfo(std::string_view message, std::string_view title = "Info");
/// Display a popup warning window with the specified message.
void popupWarning(std::string_view message, std::string_view title = "Warning");
/// Display a popup error window with the specified message.
void popupError(std::string_view message, std::string_view title = "Error");
/// Display a popup info window with the specified message, as well as Ok/Yes and Cancel/No buttons.
/// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
bool popupInfoChoice(std::string_view message, bool useYesNo = false, std::string_view title = "Info");
/// Display a popup warning window with the specified message, as well as Ok/Yes and Cancel/No buttons.
/// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
bool popupWarningChoice(std::string_view message, bool useYesNo = false, std::string_view title = "Warning");
/// Display a popup error window with the specified message, as well as Ok/Yes and Cancel/No buttons.
/// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
bool popupErrorChoice(std::string_view message, bool useYesNo = false, std::string_view title = "Error");
/// Display a popup question window with the specified question.
/// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
bool popupQuestion(std::string_view question, bool useYesNo = false, std::string_view title = "Choice");

} // namespace chira::Dialogs
