#pragma once

#include <string>
#include "abstractSettingsLoader.h"
#include "../core/virtualFileSystem.h"

class abstractFileSettingsLoader : public abstractSettingsLoader {
public:
    explicit abstractFileSettingsLoader(const std::string& path) {
        this->filepath = virtualFileSystem::getGenericPath(path, ENGINE_FILESYSTEM_PREFIX, false);
    }
    ~abstractFileSettingsLoader() override = default;
    void setFilePath(const std::string& path) {
        this->filepath = virtualFileSystem::getGenericPath(path);
    };
    std::string getFilePath() {
        return this->filepath;
    }
private:
    std::string filepath;
};
