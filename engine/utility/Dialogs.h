#pragma once

#include <string>

namespace chira::Dialogs {

/// Returns an absolute path to the file selected, or an empty string if none selected.
std::string openFile(const std::string& pattern, const std::string& description = "");
/// Returns an absolute path to the folder selected, or an empty string if none selected.
std::string openFolder();
/// Returns a relative path to the resource selected, or an empty string if none selected.
std::string openResource(const std::string& pattern, const std::string& description = "");
/// Return an absolute path to the file to create, or an empty string if Cancel pressed.
std::string saveFile(const std::string& pattern, const std::string& defaultName = "", const std::string& description = "");

/// Display a popup window with an input box.
std::string input(const std::string& message = "", const std::string& title = "Input", const std::string& placeholder = "");
/// Display a popup window with a password box.
std::string password(const std::string& message = "", const std::string& title = "Password");

/// Display a popup info window with the specified message.
void popupInfo(const std::string& message, const std::string& title = "Info");
/// Display a popup warning window with the specified message.
void popupWarning(const std::string& message, const std::string& title = "Warning");
/// Display a popup error window with the specified message.
void popupError(const std::string& message, const std::string& title = "Error");
/// Display a popup info window with the specified message, as well as Ok/Yes and Cancel/No buttons.
/// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
bool popupInfoChoice(const std::string& message, bool useYesNo = false, const std::string& title = "Info");
/// Display a popup warning window with the specified message, as well as Ok/Yes and Cancel/No buttons.
/// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
bool popupWarningChoice(const std::string& message, bool useYesNo = false, const std::string& title = "Warning");
/// Display a popup error window with the specified message, as well as Ok/Yes and Cancel/No buttons.
/// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
bool popupErrorChoice(const std::string& message, bool useYesNo = false, const std::string& title = "Error");
/// Display a popup question window with the specified question.
/// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
bool popupQuestion(const std::string& question, bool useYesNo = false, const std::string& title = "Choice");

} // namespace chira::Dialogs
