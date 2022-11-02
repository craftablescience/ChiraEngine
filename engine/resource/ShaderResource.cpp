#include "ShaderResource.h"

#include <regex>
#include <glad/glversion.h>

#include <core/Logger.h>
#include <i18n/TranslationManager.h>
#include <utility/String.h>

using namespace chira;

CHIRA_CREATE_LOG(SHADERRESOURCE);

ShaderResource::ShaderResource(std::string identifier_, int type_)
    : StringResource(std::move(identifier_))
    , HandleObject<int>()
    , type(type_) {}

void ShaderResource::compile(const byte buffer[], std::size_t bufferLength) {
    if (this->handle != -1)
        return;
    this->handle = glCreateShader(this->type);

    StringResource::compile(buffer, bufferLength);
    this->data = replaceMacros(this->identifier, this->data);
    this->data = std::string{GL_VERSION_STRING.data()} + "\n\n" + this->data;

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
        LOG_SHADERRESOURCE.error(TRF("error.shader_resource.compilation_failure", this->type, infoLog));
    }
}

std::string ShaderResource::replaceMacros(const std::string& ignoredInclude, const std::string& data) { // NOLINT(misc-no-recursion)
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
    for (std::sregex_iterator it{data.begin(), data.end(), includes}; it != std::sregex_iterator{}; it++) {
        if (it->str(2) != ignoredInclude && !ShaderResource::preprocessorSymbols.count(it->str(2))) {
            auto contents = Resource::getUniqueUncachedResource<StringResource>(it->str(2));
            ShaderResource::addPreprocessorSymbol(it->str(1), replaceMacros(it->str(2), contents->getString()));
        }
    }

    // Macros
    std::string out = data;
    for (const auto& [macro, contents] : ShaderResource::preprocessorSymbols) {
        std::string fullKey = ShaderResource::preprocessorPrefix;
        fullKey += macro;
        fullKey += ShaderResource::preprocessorSuffix;
        String::replace(out, fullKey, contents);
    }
    return out;
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
