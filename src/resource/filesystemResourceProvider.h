#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include <fstream>
#include <utility>
#include "abstractResourceProvider.h"

class filesystemResourceProvider : public abstractResourceProvider {
public:
    explicit filesystemResourceProvider(const std::string& name_, std::string path_) : abstractResourceProvider(name_), path(std::move(path_)) {}
    bool hasResource(const std::string& name) override {
        // Note: update your compiler if compilation fails because of std::filesystem
        return std::filesystem::exists(std::filesystem::current_path().string() + "/" + this->path + "/" + name);
    }
    void compileResource(const std::string& name, abstractResource* resource) override {
        std::ifstream ifs((std::filesystem::current_path().string() + "/" + this->path + "/" + name).c_str(), std::ios::in | std::ios::binary | std::ios::ate);
        std::ifstream::pos_type fileSize = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        auto* bytes = new unsigned char[(std::size_t) fileSize + 1];
        ifs.read(reinterpret_cast<char*>(bytes), fileSize);
        bytes[fileSize] = '\0';
        resource->compile(bytes, (std::size_t) fileSize + 1);
        delete[] bytes;
    }
private:
    std::string path;
};
