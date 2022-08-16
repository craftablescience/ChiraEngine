#include "ISettingsLoader.h"

#include <filesystem>

using namespace chira;

ISettingsLoader::ISettingsLoader(std::string_view path, std::string_view filename) {
    this->setFilePath(path, filename);
}

void ISettingsLoader::setFilePath(std::string_view path, std::string_view filename) {
    // Test for the paths existence first and create it if it doesn't exist
    // I have no idea where else to put this so uh yeah
    
    if (!std::filesystem::exists(std::filesystem::current_path().append(path).string())) {
        std::filesystem::create_directory(std::filesystem::current_path().append(path).string());
    }
    this->filepath = std::filesystem::current_path().append(path).string();
    this->filepath.append(filename);
}
