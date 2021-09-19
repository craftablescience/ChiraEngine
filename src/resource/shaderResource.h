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
    shaderResource(const std::string& identifier_, int type_);
    void compile(unsigned char* buffer, std::size_t bufferLength) override;
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
