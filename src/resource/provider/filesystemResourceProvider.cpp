#include "filesystemResourceProvider.h"

#include <fstream>
#include <filesystem>
#include <resource/resource.h>

using namespace chira;

bool filesystemResourceProvider::hasResource(const std::string& name) {
    // Update your compiler if compilation fails because of std::filesystem
    return std::filesystem::exists(std::filesystem::current_path().append(this->path).append(name));
}

void filesystemResourceProvider::compileResource(const std::string& name, resource* resource) {
    std::ifstream ifs((std::filesystem::current_path().append(this->path).append(name).string()).c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    auto* bytes = new unsigned char[(std::size_t) fileSize + 1];
    ifs.read(reinterpret_cast<char*>(bytes), fileSize);
    bytes[fileSize] = '\0';
    resource->compile(bytes, (std::size_t) fileSize + 1);
    delete[] bytes;
}

std::string filesystemResourceProvider::getResourcePath(const std::string& absolutePath) {
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
    return "file://" + path;
}
