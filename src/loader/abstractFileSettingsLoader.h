#pragma once

#include <string>
#include <filesystem>
#include "abstractSettingsLoader.h"
#include "../resource/resourceManager.h"

class abstractFileSettingsLoader : public abstractSettingsLoader {
public:
    explicit abstractFileSettingsLoader(const std::string& path) {
        this->filepath = std::filesystem::current_path().append(path).string();
    }
    void setFilePath(const std::string& path) {
        this->filepath = std::filesystem::current_path().append(path).string();
    }
    std::string getFilePath() {
        return this->filepath;
    }
private:
    std::string filepath;
};
