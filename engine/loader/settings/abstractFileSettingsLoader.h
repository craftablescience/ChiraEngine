#pragma once

#include "abstractSettingsLoader.h"

namespace chira {
    class AbstractFileSettingsLoader : public AbstractSettingsLoader {
    public:
        explicit AbstractFileSettingsLoader(const std::string& path);
        void setFilePath(const std::string& path);
        [[nodiscard]] std::string getFilePath() const {
            return this->filepath;
        }
    private:
        std::string filepath;
    };
}
