#pragma once

#include <utility/string/stringStrip.h>
#include "abstractResourceProvider.h"

namespace chira {
    const std::string FILESYSTEM_ROOT_FOLDER = "resources"; // NOLINT(cert-err58-cpp)
    const std::string FILESYSTEM_PROVIDER_NAME = "file"; // NOLINT(cert-err58-cpp)

    class filesystemResourceProvider : public abstractResourceProvider {
    public:
        explicit filesystemResourceProvider(const std::string& path_) :
            abstractResourceProvider(FILESYSTEM_PROVIDER_NAME), path(FILESYSTEM_ROOT_FOLDER + '/' + strip(path_, "/")) {}
        bool hasResource(const std::string& name) override;
        void compileResource(const std::string& name, resource* resource) override;
        [[nodiscard]] const std::string& getPath() const {
            return path;
        }
        /// Takes an absolute path of a resource file and converts it to a resource identifier.
        /// Does not check if the resource identifier actually points to a valid resource.
        static std::string getResourcePath(const std::string& absolutePath);
    private:
        std::string path;
    };
}
