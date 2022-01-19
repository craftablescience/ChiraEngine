#include "filesystemResourceProvider.h"

#include <fstream>
#include <filesystem>
#include <utility>
#include <resource/resource.h>

using namespace chira;

FilesystemResourceProvider::FilesystemResourceProvider(std::string path_, bool isPathAbsolute, const std::string& name_)
    : AbstractResourceProvider(name_)
    , path(std::move(path_))
    , absolute(isPathAbsolute) {
    FilesystemResourceProvider::nixifyPath(this->path);
    stripRightModify(this->path, '/');
    if (!this->absolute)
        this->path = FILESYSTEM_ROOT_FOLDER + '/' + this->path;
}

bool FilesystemResourceProvider::hasResource(const std::string& name) const {
    // Update your compiler if compilation fails because of std::filesystem
    if (this->absolute)
        return std::filesystem::exists(std::filesystem::path{this->path}.append(name));
    else
        return std::filesystem::exists(std::filesystem::current_path().append(this->path).append(name));
}

void FilesystemResourceProvider::compileResource(const std::string& name, Resource* resource) const {
    std::filesystem::path resourcePath;
    if (this->absolute)
        resourcePath = std::filesystem::path{this->path}.append(name);
    else
        resourcePath = std::filesystem::current_path().append(this->path).append(name);
    std::uintmax_t fileSize = std::filesystem::file_size(resourcePath);
    std::ifstream ifs(resourcePath.string().c_str(), std::ios::in | std::ios::binary);
    ifs.seekg(0, std::ios::beg);
    auto* bytes = new unsigned char[(std::size_t) fileSize + 1];
    ifs.read(reinterpret_cast<char*>(bytes), static_cast<std::streamsize>(fileSize));
    bytes[fileSize] = '\0';
    resource->compile(bytes, (std::size_t) fileSize + 1);
    delete[] bytes;
}

std::string FilesystemResourceProvider::getFolder() const {
    return stripLeft(this->getPath(), FILESYSTEM_ROOT_FOLDER + '/');
}

std::string FilesystemResourceProvider::getLocalResourceAbsolutePath(const std::string& identifier) const {
    // Make sure we've been passed a valid identifier
    auto name = Resource::splitResourceIdentifier(identifier).second;
    if (!this->hasResource(name))
        return "";
    auto absPath = (this->absolute ? std::filesystem::path{this->path} : std::filesystem::current_path().append(this->path)).append(name).string();
    // Replace cringe Windows-style backslashes
    FilesystemResourceProvider::nixifyPath(absPath);
    return absPath;
}

void FilesystemResourceProvider::nixifyPath(std::string& path) {
    std::replace(path.begin(), path.end(), '\\', '/');
}

std::string FilesystemResourceProvider::getResourceIdentifier(const std::string& absolutePath) {
    // Add the resource provider prefix
    if (auto path = FilesystemResourceProvider::getResourceFolderPath(absolutePath); !path.empty())
        return FILESYSTEM_PROVIDER_NAME + RESOURCE_ID_SEPARATOR.data() + path;
    return "";
}

std::string FilesystemResourceProvider::getResourceFolderPath(const std::string& absolutePath) {
    // Make sure we've been passed a valid resource path
    if (absolutePath.find(FILESYSTEM_ROOT_FOLDER) == std::string::npos)
        return "";

    std::string path = absolutePath;

    // Replace cringe Windows-style backslashes
    FilesystemResourceProvider::nixifyPath(path);

    // Remove everything before the root folder, the root folder, and the forward slash
    auto index = path.rfind(FILESYSTEM_ROOT_FOLDER) + FILESYSTEM_ROOT_FOLDER.size() + 1;
    path = path.substr(index);

    // Remove the namespace
    index = path.find('/') + 1;
    path = path.substr(index);
    return path;
}

std::string FilesystemResourceProvider::getResourceAbsolutePath(const std::string& identifier) {
    if (auto provider = Resource::getResourceProviderWithResource(identifier))
        return assert_cast<FilesystemResourceProvider*>(provider)->getLocalResourceAbsolutePath(identifier);
    return "";
}
