#include "filesystemResourceProvider.h"

#include <fstream>
#include <filesystem>
#include <resource/resource.h>

using namespace chira;

FilesystemResourceProvider::FilesystemResourceProvider(const std::string& path_) :
        AbstractResourceProvider(FILESYSTEM_PROVIDER_NAME),
        path(FILESYSTEM_ROOT_FOLDER + '/' + strip(path_, "/")) {}

bool FilesystemResourceProvider::hasResource(const std::string& name) const {
    // Update your compiler if compilation fails because of std::filesystem
    return std::filesystem::exists(std::filesystem::current_path().append(this->path).append(name));
}

void FilesystemResourceProvider::compileResource(const std::string& name, Resource* resource) const {
    std::ifstream ifs((std::filesystem::current_path().append(this->path).append(name).string()).c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    auto* bytes = new unsigned char[(std::size_t) fileSize + 1];
    ifs.read(reinterpret_cast<char*>(bytes), fileSize);
    bytes[fileSize] = '\0';
    resource->compile(bytes, (std::size_t) fileSize + 1);
    delete[] bytes;
}

std::string FilesystemResourceProvider::getAbsoluteResourcePath(const std::string& identifier) const {
    // Make sure we've been passed a valid identifier
    auto name = Resource::splitResourceIdentifier(identifier).second;
    if (!this->hasResource(name))
        return "";
    auto absPath = std::filesystem::current_path().append(this->path).append(name).string();
    // Replace cringe Windows-style backslashes
    std::replace_if(absPath.begin(), absPath.end(), []( char c ){ return c == '\\'; }, '/');
    return absPath;
}

std::string FilesystemResourceProvider::getResourcePath(const std::string& absolutePath) {
    // Make sure we've been passed a valid resource path
    if (absolutePath.find(FILESYSTEM_ROOT_FOLDER) == std::string::npos)
        return "";

    std::string path = absolutePath;

    // Replace cringe Windows-style backslashes
    std::replace_if(path.begin(), path.end(), []( char c ){ return c == '\\'; }, '/');

    // Remove everything before the root folder, the root folder, and the forward slash
    auto index = path.rfind(FILESYSTEM_ROOT_FOLDER) + FILESYSTEM_ROOT_FOLDER.size() + 1;
    path = path.substr(index);

    // Remove the namespace
    index = path.find('/') + 1;
    path = path.substr(index);

    // Add the resource provider
    return FILESYSTEM_PROVIDER_NAME + RESOURCE_ID_SEPARATOR.data() + path;
}
