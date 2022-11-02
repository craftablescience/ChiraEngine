#include "ISettingsLoader.h"

#include <filesystem>

using namespace chira;

ISettingsLoader::ISettingsLoader(std::string_view filename, std::string_view path, bool relative /*= false*/) {
    std::string pathStr;
    if (relative) {
        pathStr = std::filesystem::current_path().append(path).string();
    } else {
        pathStr = path;
    }

    // Test for the path's existence and create it if it doesn't exist
    if (!std::filesystem::exists(pathStr)) {
        std::filesystem::create_directory(pathStr);
    }
    this->filepath = pathStr;
    this->filepath.append(filename);
}
