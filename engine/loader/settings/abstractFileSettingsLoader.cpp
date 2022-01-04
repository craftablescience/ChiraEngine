#include "abstractFileSettingsLoader.h"

#include <filesystem>

using namespace chira;

AbstractFileSettingsLoader::AbstractFileSettingsLoader(const std::string& path) {
    this->setFilePath(path);
}

void AbstractFileSettingsLoader::setFilePath(const std::string& path) {
    this->filepath = std::filesystem::current_path().append(path).string();
}
