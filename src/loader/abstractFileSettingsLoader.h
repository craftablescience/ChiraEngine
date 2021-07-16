#ifndef BASICGAMEENGINE_ABSTRACTFILESETTINGSLOADER_H
#define BASICGAMEENGINE_ABSTRACTFILESETTINGSLOADER_H


#include <string>
#include "abstractSettingsLoader.h"

class abstractFileSettingsLoader : public abstractSettingsLoader {
public:
    explicit abstractFileSettingsLoader(const std::string& path) {
        this->filepath = path;
    }
    ~abstractFileSettingsLoader() override = default;
    void setFilePath(const std::string& path) {
        this->filepath = path;
    };
    std::string getFilePath() {
        return this->filepath;
    }
private:
    std::string filepath;
};


#endif //BASICGAMEENGINE_ABSTRACTFILESETTINGSLOADER_H
