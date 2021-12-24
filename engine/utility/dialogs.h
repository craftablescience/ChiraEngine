#pragma once

#include <string>

namespace chira {
    struct dialogs {
        /// Returns an absolute path to the file selected, or an empty string if none selected.
        static std::string openFile(const std::string& pattern, const std::string& description = "");
        /// Returns a relative path to the resource selected, or an empty string if none selected.
        static std::string openResource(const std::string& pattern, const std::string& description = "");

        /// Display a popup info window with the specified message.
        static void popupInfo(const std::string& message);
        /// Display a popup warning window with the specified message.
        static void popupWarning(const std::string& message);
        /// Display a popup error window with the specified message.
        static void popupError(const std::string& message);
        /// Display a popup question window with the specified question.
        /// Returns true if Ok/Yes pressed, false if No/Cancel pressed.
        static bool popupQuestion(const std::string& question, bool useYesNo = false, const std::string& title = "Choice");
    };
}
