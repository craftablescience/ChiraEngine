#pragma once

#include <utility/string/stringStrip.h>
#include "abstractResourceProvider.h"

namespace chira {
    const std::string FILESYSTEM_ROOT_FOLDER = "resources"; // NOLINT(cert-err58-cpp)
    const std::string FILESYSTEM_PROVIDER_NAME = "file"; // NOLINT(cert-err58-cpp)

    class FilesystemResourceProvider : public AbstractResourceProvider {
    public:
        explicit FilesystemResourceProvider(std::string path_, bool isPathAbsolute = false, const std::string& name_ = FILESYSTEM_PROVIDER_NAME);
        [[nodiscard]] bool hasResource(const std::string& name) const override;
        void compileResource(const std::string& name, Resource* resource) const override;
        [[nodiscard]] const std::string& getPath() const {
            return this->path;
        }
        [[nodiscard]] bool isAbsolute() const {
            return this->absolute;
        }
        [[nodiscard]] std::string getFolder() const;
        [[nodiscard]] std::string getLocalResourceAbsolutePath(const std::string& identifier) const;

        /// Converts all backslashes in a string to forward slashes.
        static void nixifyPath(std::string& path);
        /// Takes an absolute path of a resource file and converts it to a resource identifier.
        /// Does not check if the resource identifier actually points to a valid resource.
        static std::string getResourceIdentifier(const std::string& absolutePath);
        /// Takes an absolute path of a resource folder and converts it to a valid input path
        /// for a FilesystemResourceProvider. Does not check if the resource identifier
        /// actually points to a valid resource.
        static std::string getResourceFolderPath(const std::string& absolutePath);
        /// Takes a resource identifier and returns the full absolute path, if it exists.
        static std::string getResourceAbsolutePath(const std::string& identifier);

        static inline constexpr short FILEPATH_MAX_LENGTH = 1024;
    private:
        std::string path;
        bool absolute;
    };
}
