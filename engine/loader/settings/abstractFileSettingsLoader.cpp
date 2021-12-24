#include "abstractFileSettingsLoader.h"

#include <filesystem>

using namespace chira;

abstractFileSettingsLoader::abstractFileSettingsLoader(const std::string& path) {
    this->setFilePath(path);
}

void abstractFileSettingsLoader::setFilePath(const std::string& path) {
    this->filepath = std::filesystem::current_path().append(path).string();
}