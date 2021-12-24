#pragma once

#include <string>

namespace chira {
    /// Returns an absolute path to the file selected, or an empty string if none selected.
    std::string dialogOpenFile(const std::string& pattern, const std::string& description = "");
    /// Returns a relative path to the resource selected, or an empty string if none selected.
    std::string dialogOpenResource(const std::string& pattern, const std::string& description = "");

    /// Display a popup info window with the specified message.
    void dialogPopupInfo(const std::string& message, const std::string& title = "Info");
    /// Display a popup warning window with the specified message.
    void dialogPopupWarning(const std::string& message, const std::string& title = "Warning");
    /// Display a popup error window with the specified message.
    void dialogPopupError(const std::string& message, const std::string& title = "Error");
    /// Display a popup info window with the specified message, as well as Ok/Yes and Cancel/No buttons.
    /// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
    bool dialogPopupInfoChoice(const std::string& message, bool useYesNo = false, const std::string& title = "Info");
    /// Display a popup warning window with the specified message, as well as Ok/Yes and Cancel/No buttons.
    /// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
    bool dialogPopupWarningChoice(const std::string& message, bool useYesNo = false, const std::string& title = "Warning");
    /// Display a popup error window with the specified message, as well as Ok/Yes and Cancel/No buttons.
    /// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
    bool dialogPopupErrorChoice(const std::string& message, bool useYesNo = false, const std::string& title = "Error");
    /// Display a popup question window with the specified question.
    /// Returns true if Ok/Yes pressed, false if Cancel/No pressed.
    bool dialogPopupQuestion(const std::string& question, bool useYesNo = false, const std::string& title = "Choice");
}
