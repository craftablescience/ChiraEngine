#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <utility>
#include "abstractResourceProvider.h"
#include "../../utility/string/stringStrip.h"

namespace chira {
    class filesystemResourceProvider : public abstractResourceProvider {
    public:
        explicit filesystemResourceProvider(const std::string& name_, std::string path_) : abstractResourceProvider(strip(name_, "/")), path(std::move(path_)) {}
        bool hasResource(const std::string& name) override;
        void compileResource(const std::string& name, abstractResource* resource) override;
        [[nodiscard]] const std::string& getPath() const {
            return path;
        }
    private:
        std::string path;
    };
}
