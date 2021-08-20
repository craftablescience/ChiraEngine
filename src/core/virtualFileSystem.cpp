#include "virtualFileSystem.h"
#include <fstream>

std::vector<std::string>* virtualFileSystem::getResourceDirectories() {
    return &(virtualFileSystem::resourcePaths);
}

void virtualFileSystem::addResourceDirectory(const std::string& name) {
    virtualFileSystem::resourcePaths.push_back(name);
}

std::string virtualFileSystem::getShaderPath(const std::string& path, const std::string& fallbackPrefix, bool error) {
    return virtualFileSystem::getGenericPath("shaders/" + path, fallbackPrefix, error);
}

std::string virtualFileSystem::getTexturePath(const std::string& path, const std::string& fallbackPrefix, bool error) {
    return virtualFileSystem::getGenericPath("textures/" + path, fallbackPrefix, error);
}

std::string virtualFileSystem::getMeshPath(const std::string& path, const std::string& fallbackPrefix, bool error) {
    return virtualFileSystem::getGenericPath("meshes/" + path, fallbackPrefix, error);
}

std::string virtualFileSystem::getSoundPath(const std::string& path, const std::string& fallbackPrefix, bool error) {
    return virtualFileSystem::getGenericPath("sounds/" + path, fallbackPrefix, error);
}

std::string virtualFileSystem::getScriptPath(const std::string& path, const std::string& fallbackPrefix, bool error) {
    return virtualFileSystem::getGenericPath("scripts/" + path, fallbackPrefix, error);
}

std::string virtualFileSystem::getGenericPath(const std::string& path, const std::string& fallbackPrefix, bool error) {
    for (auto pathBeginning = virtualFileSystem::resourcePaths.rbegin();
         pathBeginning != virtualFileSystem::resourcePaths.rend();
         pathBeginning++) {
        if (virtualFileSystem::fileExists(*pathBeginning + path)) {
            return *pathBeginning + path;
        }
    }
    if (error) {
        chiraLogger::log(WARN, "VFS", "Path " + path + " does not exist");
    }
    return fallbackPrefix + path;
}

bool virtualFileSystem::fileExists(const std::string& fullPath) {
    std::ifstream file(fullPath);
    return (bool) file;
}
