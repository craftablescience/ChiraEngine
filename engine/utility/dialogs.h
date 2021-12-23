#pragma once

#include <string>

namespace chira {
    /// Returns an absolute path to the file selected, or an empty string if none selected.
    std::string openFile(const std::string& pattern, const std::string& description = "");
    /// Returns a relative path to the resource selected, or an empty string if none selected.
    std::string openResource(const std::string& pattern, const std::string& description = "");
}
