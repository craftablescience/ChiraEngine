#include "FilesystemResourceProvider.h"

#include <fstream>
#include <filesystem>
#include <utility>
#include <resource/Resource.h>
#include <core/Engine.h>

using namespace chira;

CHIRA_CREATE_LOG(FILESYSTEM);

FilesystemResourceProvider::FilesystemResourceProvider(std::string path_, bool isPathAbsolute, const std::string& name_)
    : IResourceProvider(name_)
    , path(std::move(path_))
    , absolute(isPathAbsolute) {
    FilesystemResourceProvider::nixifyPath(this->path);
    this->path = String::stripRight(this->path, '/');
    if (!this->absolute) {
    // MacOS is a lot more complex. There's more than 1 place our resources could be at
    // One requires a bit of work to access though.
#if defined(__APPLE__) && defined(__MACH__)
        // Finding the bundle and it's resources directory
        std::string bundlePath = Engine::getBundleDir();
        std::string append = "/Contents/Resource";
        if (std::filesystem::exists(std::filesystem::path{bundlePath + append + FILESYSTEM_ROOT_FOLDER + '/' + this->path})) {
            LOG_FILESYSTEM.info("Found resources in app bundle!");
            this->path = bundlePath + append + FILESYSTEM_ROOT_FOLDER + '/' + this->path;
        }
        else if (std::filesystem::exists(std::filesystem::path{FILESYSTEM_ROOT_FOLDER + '/' + this->path})) {
            LOG_FILESYSTEM.info("Found resources in working directory!");
            this->path = FILESYSTEM_ROOT_FOLDER + '/' + this->path;
        }
        else {
            throw std::runtime_error{"[FILESYSTEM] FATAL ERROR: No known search path contains our requires resources! Did you mistype something?"};
        }
#else
        // Other platforms can just look in the working directory
        this->path = FILESYSTEM_ROOT_FOLDER + '/' + this->path;
#endif
    }
}

bool FilesystemResourceProvider::hasResource(std::string_view name) const {
    // Update your compiler if compilation fails because of std::filesystem
    if (this->absolute)
        return std::filesystem::exists(std::filesystem::path{this->path}.append(name));
    else
        return std::filesystem::exists(std::filesystem::current_path().append(this->path).append(name));
}

void FilesystemResourceProvider::compileResource(std::string_view name, Resource* resource) const {
    std::filesystem::path resourcePath;
    if (this->absolute)
        resourcePath = std::filesystem::path{this->path}.append(name);
    else
        resourcePath = std::filesystem::current_path().append(this->path).append(name);
    std::uintmax_t fileSize = std::filesystem::file_size(resourcePath);
    std::ifstream ifs(resourcePath.string().c_str(), std::ios::in | std::ios::binary);
    ifs.seekg(0, std::ios::beg);
    auto* bytes = new byte[(std::size_t) fileSize + 1];
    ifs.read(reinterpret_cast<char*>(bytes), static_cast<std::streamsize>(fileSize));
    bytes[fileSize] = '\0';
    resource->compile(bytes, (std::size_t) fileSize + 1);
    delete[] bytes;
}

std::string FilesystemResourceProvider::getFolder() const {
    return String::stripLeft(std::string{this->getPath().data()}, FILESYSTEM_ROOT_FOLDER + '/');
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

std::string FilesystemResourceProvider::getResourceIdentifier(std::string_view absolutePath) {
    // Add the resource provider prefix
    if (auto path = FilesystemResourceProvider::getResourceFolderPath(absolutePath); !path.empty())
        return FILESYSTEM_PROVIDER_NAME + RESOURCE_ID_SEPARATOR.data() + path;
    return "";
}

std::string FilesystemResourceProvider::getResourceFolderPath(std::string_view absolutePath) {
    // Make sure we've been passed a valid resource path
    if (absolutePath.find(FILESYSTEM_ROOT_FOLDER) == std::string_view::npos)
        return "";

    std::string path{absolutePath.data()};

    // Replace cringe Windows-style backslashes
    FilesystemResourceProvider::nixifyPath(path);

    // Remove everything before the root folder, the root folder, and the forward slash
    auto index = path.rfind(FILESYSTEM_ROOT_FOLDER) + FILESYSTEM_ROOT_FOLDER.size() + 1;
    if (index > path.length())
        return "";
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

std::map<std::string, fileInfo> FilesystemResourceProvider::getDirectoryContents(std::string path, std::string extension = "") {
    // if an error happens AT ANY POINT we still need to pass something over so we can cancel the search
    // right at the point of failure if need be. so we pass an empty map back to get around that.
    std::map<std::string, fileInfo> DUMMY;
    
    // the actual map we populate with files
    std::map<std::string, fileInfo> files;
    
    // Check if the directory exists first
    // FIXME: this check breaks if the path has a trailing '/'
    auto resourceFolderPath = FilesystemResourceProvider::getResourceFolderPath(path);
    if (resourceFolderPath.empty()) {
        LOG_FILESYSTEM.error("Resource directory " + path + " does not exist!");
        return DUMMY;
    }
    
    // grab the contents of this directory and put the files into a file table
    // TODO: Comment this block.
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        std::string filename = entry.path().stem().string();
        fileInfo filedef;
        if (entry.is_directory())
            filedef.fileType = FILE_DIRECTORY;
        else {
            std::string ext = entry.path().extension().string();
            // TODO: add other file extensions to this list
            if (ext == ".mdef") // Mesh definition
                filedef.fileType = FILE_MESH;
            else // Default/Unknown
                filedef.fileType = FILE_GENERIC;
        }
        if (!entry.is_directory())
            filename+=entry.path().extension().string();
        filedef.fileName = filename;
        filedef.filePath = entry.path().string();
        files[ filename ] = filedef;
        LOG_FILESYSTEM.info("Found File/Dir Entry " + filename);
    }
    LOG_FILESYSTEM.info("Finished searching directory");
    return files;
}
