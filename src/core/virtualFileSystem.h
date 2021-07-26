#pragma once

#include <vector>
#include <string>
#include "engine.h"

const std::string ENGINE_FILESYSTEM_PREFIX = "resources/engine/";

class virtualFileSystem {
public:
    static std::vector<std::string>* getResourceDirectories();
    static void addResourceDirectory(const std::string& name);
    static std::string getShaderPath(const std::string& path, const std::string& fallbackPrefix = ENGINE_FILESYSTEM_PREFIX, bool error = false);
    static std::string getTexturePath(const std::string& path, const std::string& fallbackPrefix = ENGINE_FILESYSTEM_PREFIX, bool error = false);
    static std::string getMeshPath(const std::string& path, const std::string& fallbackPrefix = ENGINE_FILESYSTEM_PREFIX, bool error = false);
    static std::string getSoundPath(const std::string& path, const std::string& fallbackPrefix = ENGINE_FILESYSTEM_PREFIX, bool error = false);
    static std::string getScriptPath(const std::string& path, const std::string& fallbackPrefix = ENGINE_FILESYSTEM_PREFIX, bool error = false);
    static std::string getGenericPath(const std::string& path, const std::string& fallbackPrefix = ENGINE_FILESYSTEM_PREFIX, bool error = false);
private:
    static inline std::vector<std::string> resourcePaths{};
    static bool fileExists(const std::string& fullPath);
};
