#include "ISettingsLoader.h"

#include <filesystem>

using namespace chira;

ISettingsLoader::ISettingsLoader(const std::string& path) {
    this->setFilePath(path);
}

void ISettingsLoader::setFilePath(const std::string& path) {
    this->filepath = std::filesystem::current_path().append(path).string();
}
