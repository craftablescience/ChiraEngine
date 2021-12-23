#pragma once

#include <utility/string/stringStrip.h>
#include "abstractResourceProvider.h"

namespace chira {
    const std::string FILESYSTEM_ROOT_FOLDER = "resources"; // NOLINT(cert-err58-cpp)

    class filesystemResourceProvider : public abstractResourceProvider {
    public:
        filesystemResourceProvider(const std::string& name_, const std::string& path_) :
            abstractResourceProvider(name_), path(FILESYSTEM_ROOT_FOLDER + '/' + strip(path_, "/")) {}
        bool hasResource(const std::string& name) override;
        void compileResource(const std::string& name, resource* resource) override;
        [[nodiscard]] const std::string& getPath() const {
            return path;
        }
        static std::string getResourcePath(const std::string& absolutePath);
    private:
        std::string path;
    };
}
