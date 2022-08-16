#pragma once

#include <utility/String.h>
#include <map>
#include "IResourceProvider.h"

namespace chira {

const std::string FILESYSTEM_ROOT_FOLDER = "resources"; // NOLINT(cert-err58-cpp)
const std::string FILESYSTEM_PROVIDER_NAME = "file"; // NOLINT(cert-err58-cpp)

class FilesystemResourceProvider : public IResourceProvider {
public:
    explicit FilesystemResourceProvider(std::string path_, bool isPathAbsolute = false, const std::string& name_ = FILESYSTEM_PROVIDER_NAME);
    [[nodiscard]] bool hasResource(std::string_view name) const override;
    void compileResource(std::string_view name, Resource* resource) const override;
    [[nodiscard]] std::string_view getPath() const {
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
    static std::string getResourceIdentifier(std::string_view absolutePath);
    /// Takes an absolute path of a resource folder and converts it to a valid input path
    /// for a FilesystemResourceProvider. Does not check if the resource identifier
    /// actually points to a valid resource.
    static std::string getResourceFolderPath(std::string_view absolutePath);
    /// Takes a resource identifier and returns the full absolute path, if it exists.
    static std::string getResourceAbsolutePath(const std::string& identifier);
    /// Takes a directory and pulls every file inside of it into a filetype table
    static std::map<std::string, std::string[3]> getDirectoryContents(std::string path);

    static constexpr inline short FILEPATH_MAX_LENGTH = 1024;
private:
    std::string path;
    bool absolute;
};

enum FileType {
    FILE_GENERIC    = 0 << 1, // Unknown, Unsupported file type
    FILE_SCRIPT     = 0 << 2, // AngelScript file
    FILE_MODEL      = 0 << 3, // Supported model type
    FILE_MESH       = 0 << 4, // Complete mesh definition file
    FILE_AUDIO      = 0 << 5, // Supported Audio Format
    FILE_IMAGE      = 0 << 6, // Supported Image Format
    FILE_MATERIAL   = 0 << 7, // Material Definition
    FILE_FONT       = 0 << 8, // Font definition
    FILE_TEXTURE    = 0 << 9, // Texture definition
};

struct fileInfo {
    std::string fileName;
    FileType fileType;
    /// filePath: non absolute resource relative path
    /// EX: file://engine/meshes/missing.json
    std::string filePath;
};


} // namespace chira
