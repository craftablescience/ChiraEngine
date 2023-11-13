#include "Shader.h"

#include <regex>

#include <core/debug/Logger.h>
#include <core/utility/String.h>
#include <resource/StringResource.h>
#include "UBO.h"

using namespace chira;

CHIRA_CREATE_LOG(SHADER);

Shader::Shader(std::string path_)
        : Resource(std::move(path_)) {}

void Shader::compile(const std::byte buffer[], std::size_t bufferLength) {
    Serial::loadFromBuffer(this, buffer, bufferLength);

    const auto shaderModuleVertString = Resource::getUniqueUncachedResource<StringResource>(this->vertexPath);
    const auto shaderModuleVertData = replaceMacros(shaderModuleVertString->getPath().data(), shaderModuleVertString->getString());
    const auto shaderModuleFragString = Resource::getUniqueUncachedResource<StringResource>(this->fragmentPath);
    const auto shaderModuleFragData = replaceMacros(shaderModuleFragString->getPath().data(), shaderModuleFragString->getString());
    this->handle = Renderer::createShader(shaderModuleVertData, shaderModuleFragData);

    if (this->usesPV) {
        PerspectiveViewUBO::get().bindToShader(this->handle);
    }
    if (this->lit) {
        LightsUBO::get().bindToShader(this->handle);
    }
}

void Shader::use() const {
    Renderer::useShader(this->handle);
}

Shader::~Shader() {
    Renderer::destroyShader(this->handle);
}

void Shader::addPreprocessorSymbol(const std::string& name, const std::string& value) {
    Shader::preprocessorSymbols[name] = value;
}

void Shader::setPreprocessorPrefix(const std::string& prefix) {
    Shader::preprocessorPrefix = prefix;
}

void Shader::setPreprocessorSuffix(const std::string& suffix) {
    Shader::preprocessorSuffix = suffix;
}

std::string Shader::replaceMacros(const std::string& ignoredInclude, const std::string& data) { // NOLINT(misc-no-recursion)
    // WARNING: The following code is HYPER SENSITIVE
    // If you change ANYTHING it will BREAK HORRIBLY
    // TEST ALL CHANGES

    // Includes
    static const std::regex includes{Shader::preprocessorPrefix + "(include[ \t]+([a-z:\\/.]+))" + Shader::preprocessorSuffix,
                                     std::regex_constants::icase | std::regex_constants::optimize};
    // Add the include as a macro to be expanded
    // This has the positive side effect of caching previously used includes
    for (std::sregex_iterator it{data.begin(), data.end(), includes}; it != std::sregex_iterator{}; ++it) {
        if (it->str(2) != ignoredInclude && !Shader::preprocessorSymbols.count(it->str(2))) {
            auto contents = Resource::getUniqueUncachedResource<StringResource>(it->str(2));
            Shader::addPreprocessorSymbol(it->str(1), replaceMacros(it->str(2), contents->getString()));
        }
    }

    // Macros
    std::string out = data;
    for (const auto& [macro, contents] : Shader::preprocessorSymbols) {
        std::string fullKey = Shader::preprocessorPrefix;
        fullKey += macro;
        fullKey += Shader::preprocessorSuffix;
        String::replace(out, fullKey, contents);
    }
    return out;
}
