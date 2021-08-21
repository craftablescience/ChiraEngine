#pragma once

#include <string>
#include "abstractSettingsLoader.h"
#include "../resource/filesystemResourceProvider.h"
#include "../resource/resourceManager.h"

class abstractFileSettingsLoader : public abstractSettingsLoader {
public:
    explicit abstractFileSettingsLoader(const std::string& path) {
        // todo: get file path in a better way
        this->filepath = ((filesystemResourceProvider*) resourceManager::getLatestResourceProvider("file"))->getPath() + "/" + path;
    }
    void setFilePath(const std::string& path) {
        // todo: get file path in a better way
        this->filepath = ((filesystemResourceProvider*) resourceManager::getLatestResourceProvider("file"))->getPath() + "/" + path;
    }
    std::string getFilePath() {
        return this->filepath;
    }
private:
    std::string filepath;
};
