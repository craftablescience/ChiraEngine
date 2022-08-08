#include "ShaderResource.h"

#include <sstream>
#include <regex>

#include <config/Config.h>
#include <core/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

ShaderResource::ShaderResource(std::string identifier_, int type_)
    : StringResource(std::move(identifier_))
    , HandleObject<int>()
    , type(type_) {}

void ShaderResource::compile(const byte buffer[], std::size_t bufferLength) {
    if (this->handle != -1)
        return;
    this->handle = glCreateShader(this->type);

    StringResource::compile(buffer, bufferLength);
    this->data = std::string{GL_VERSION_STRING} + "\n\n" + this->data;

    // WARNING: The following code is HYPER SENSITIVE
    // If you change ANYTHING it will BREAK HORRIBLY
    // TEST ALL CHANGES

    // Includes
    static const std::regex includes{ShaderResource::preprocessorPrefix +
                                     "(include[ \t]+([a-z:\\/.]+))" +
                                     ShaderResource::preprocessorSuffix,
                                     std::regex_constants::icase | std::regex_constants::optimize};
    // Add the include as a macro to be expanded
    // This has the positive side effect of caching previously used includes
    for (std::sregex_iterator it{this->data.begin(), this->data.end(), includes}; it != std::sregex_iterator{}; it++) {
        if (it->str(2) == this->identifier)
            continue;
        if (ShaderResource::preprocessorSymbols.count(it->str(2)) == 0) {
            auto contents = Resource::getUniqueUncachedResource<StringResource>(it->str(2));
            ShaderResource::addPreprocessorSymbol(it->str(1), contents->getString());
        }
    }

    // Macros
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
    if (this->handle != -1)
        glDeleteShader(this->handle);
}

void ShaderResource::checkForCompilationErrors() const {
    int success;
    char infoLog[512];
    glGetShaderiv(this->handle, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(this->handle, 512, nullptr, infoLog);
        Logger::log(LogType::LOG_ERROR, "Shader Resource", TRF("error.shader_resource.compilation_failure", this->type, infoLog));
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
