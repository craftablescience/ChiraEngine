#include "shaderResource.h"

#include <utility/logger.h>
#include <i18n/translationManager.h>
#include <config/glVersion.h>
#include <sstream>
#include <regex>

using namespace chira;

std::string ShaderResource::preprocessorPrefix = std::string{SHADER_PREPROCESSOR_DEFAULT_PREFIX}; // NOLINT(cert-err58-cpp)
std::string ShaderResource::preprocessorSuffix = std::string{SHADER_PREPROCESSOR_DEFAULT_SUFFIX}; // NOLINT(cert-err58-cpp)
std::unordered_map<std::string, std::string> ShaderResource::preprocessorSymbols{};

// todo: add #include preprocessing
ShaderResource::ShaderResource(const std::string& identifier_, int type_) : Resource(identifier_), HandleObject<int>(), type(type_) {}

void ShaderResource::compile(const unsigned char buffer[], std::size_t bufferLength) {
    if (this->handle != -1) return;
    this->handle = glCreateShader(type);
    std::ostringstream oBuffer;
    oBuffer << GL_VERSION_STRING << "\n\n" << buffer;
    this->data = oBuffer.str();
    for (const auto& [key, value] : ShaderResource::preprocessorSymbols) {
        std::string fullKey = ShaderResource::preprocessorPrefix;
        fullKey += key;
        fullKey += ShaderResource::preprocessorSuffix;
        this->data = std::regex_replace(this->data.data(), std::regex{fullKey}, value);
    }
    const char* dat = this->data.c_str();
    glShaderSource(this->handle, 1, &dat, nullptr);
    glCompileShader(this->handle);
#if DEBUG
    this->checkForCompilationErrors();
#endif
}

ShaderResource::~ShaderResource() {
    if (this->handle != -1) glDeleteShader(this->handle);
}

void ShaderResource::checkForCompilationErrors() const {
    int success;
    char infoLog[512];
    glGetShaderiv(this->handle, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(this->handle, 512, nullptr, infoLog);
        Logger::log(LogType::ERROR, "Shader Resource", fmt::format(TR("error.shader_resource.compilation_failure"), this->type, infoLog));
    }
}

unsigned int ShaderResource::getType() const {
    return this->type;
}

void ShaderResource::addPreprocessorSymbol(const std::string& name, const std::string& value) {
    if (ShaderResource::preprocessorSymbols.count(name) == 0) {
        ShaderResource::preprocessorSymbols.insert(std::pair<std::string, std::string>{name, value});
    } else {
        ShaderResource::preprocessorSymbols[name] = value;
    }
}

void ShaderResource::setPreprocessorPrefix(const std::string& prefix) {
    ShaderResource::preprocessorPrefix = prefix;
}

void ShaderResource::setPreprocessorSuffix(const std::string& suffix) {
    ShaderResource::preprocessorSuffix = suffix;
}
