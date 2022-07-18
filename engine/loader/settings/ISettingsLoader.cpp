#include "ISettingsLoader.h"

#include <filesystem>

using namespace chira;

ISettingsLoader::ISettingsLoader(std::string_view path) {
    this->setFilePath(path);
}

void ISettingsLoader::setFilePath(std::string_view path) {
    this->filepath = std::filesystem::current_path().append(path).string();
}
