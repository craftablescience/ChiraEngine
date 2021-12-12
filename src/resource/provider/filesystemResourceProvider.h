#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <utility>
#include <utility/string/stringStrip.h>
#include "abstractResourceProvider.h"

namespace chira {
    class filesystemResourceProvider : public abstractResourceProvider {
    public:
        filesystemResourceProvider(const std::string& name_, const std::string& path_) : abstractResourceProvider(name_), path(strip(path_, "/")) {}
        bool hasResource(const std::string& name) override;
        void compileResource(const std::string& name, resource* resource) override;
        [[nodiscard]] const std::string& getPath() const {
            return path;
        }
    private:
        std::string path;
    };
}
