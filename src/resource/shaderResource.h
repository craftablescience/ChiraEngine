#pragma once

#include <string>
#include <unordered_map>
#include <glad/gl.h>
#include <fstream>
#include <regex>
#include "../utility/handleObject.h"
#include "../resource/abstractResource.h"

const std::string PREPROCESSOR_DEFAULT_PREFIX = "#";
const std::string PREPROCESSOR_DEFAULT_SUFFIX = "#";

class shaderResource : public abstractResource, public handleObject {
public:
    shaderResource(const std::string& provider_, const std::string& name_, unsigned int type_);
    void compile(std::unique_ptr<unsigned char> buffer, unsigned int bufferLength) override {
        if (this->handle != -1) return;
        this->handle = glCreateShader(type);
        std::ostringstream oBuffer;
        oBuffer << buffer.get();
        this->data = oBuffer.str();
        for (const auto& [key, value] : shaderResource::preprocessorSymbols) {
            std::string fullKey = shaderResource::preprocessorPrefix;
            fullKey += key;
            fullKey += shaderResource::preprocessorSuffix;
            this->data = std::regex_replace(this->data.data(), std::regex{fullKey}, value);
        }
        const char* dat = this->data.c_str();
        glShaderSource(this->handle, 1, &dat, nullptr);
        glCompileShader(this->handle);
#if DEBUG
        this->checkForCompilationErrors();
#endif
    }
    ~shaderResource() override;
    [[nodiscard]] unsigned int getType() const;
    static void addPreprocessorSymbol(const std::string& name, const std::string& value);
    static void setPreprocessorPrefix(const std::string& prefix);
    static void setPreprocessorSuffix(const std::string& suffix);
private:
    unsigned int type;
    std::string data{};
    static inline std::string preprocessorPrefix = PREPROCESSOR_DEFAULT_PREFIX;
    static inline std::string preprocessorSuffix = PREPROCESSOR_DEFAULT_SUFFIX;
    static inline std::unordered_map<std::string, std::string> preprocessorSymbols{};
    void checkForCompilationErrors() const;
};
