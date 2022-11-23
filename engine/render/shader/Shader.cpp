#include "Shader.h"

#include <regex>
#include <glm/gtc/type_ptr.hpp>
#include <core/Logger.h>
#include <i18n/TranslationManager.h>
#include <resource/StringResource.h>
#include <utility/String.h>
#include "UBO.h"

using namespace chira;

CHIRA_CREATE_LOG(SHADER);

Shader::Shader(std::string identifier_)
    : PropertiesResource(std::move(identifier_))
    , HandleObject<int>() {}

void Shader::compile(const nlohmann::json& properties) {
    this->handle = glCreateProgram();
    Serialize::fromJSON(this, properties);
    glLinkProgram(this->handle);
#if DEBUG
    int success = 0;
    char infoLog[512];
    glGetProgramiv(this->handle, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->handle, 512, nullptr, infoLog);
        LOG_SHADER.error(TRF("error.opengl.shader_linking", infoLog));
    }
#endif
    if (this->usesPV) {
        PerspectiveViewUBO::get()->bindToShader(this);
    }
    if (this->lit) {
        LightsUBO::get()->bindToShader(this);
    }
}

void Shader::use() const {
    glUseProgram(this->handle);
}

Shader::~Shader() {
    for (const auto shaderHandle : this->shaderHandles) {
        if (shaderHandle != -1)
            Renderer::destroyShader(shaderHandle);
    }
    glDeleteProgram(this->handle);
}

void Shader::setUniform(std::string_view name, bool value) const {
    glUniform1i(glGetUniformLocation(this->handle, name.data()), static_cast<int>(value));
}

void Shader::setUniform(std::string_view name, int value) const {
    glUniform1i(glGetUniformLocation(this->handle, name.data()), value);
}

void Shader::setUniform(std::string_view name, unsigned int value) const {
    glUniform1ui(glGetUniformLocation(this->handle, name.data()), value);
}

void Shader::setUniform(std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(this->handle, name.data()), value);
}

void Shader::setUniform(std::string_view name, glm::vec2b value) const {
    glUniform2i(glGetUniformLocation(this->handle, name.data()), static_cast<int>(value.x), static_cast<int>(value.y));
}

void Shader::setUniform(std::string_view name, glm::vec2ui value) const {
    glUniform2ui(glGetUniformLocation(this->handle, name.data()), value.x, value.y);
}

void Shader::setUniform(std::string_view name, glm::vec2i value) const {
    glUniform2i(glGetUniformLocation(this->handle, name.data()), value.x, value.y);
}

void Shader::setUniform(std::string_view name, glm::vec2f value) const {
    glUniform2f(glGetUniformLocation(this->handle, name.data()), value.x, value.y);
}

void Shader::setUniform(std::string_view name, glm::vec3b value) const {
    glUniform3i(glGetUniformLocation(this->handle, name.data()), static_cast<int>(value.x), static_cast<int>(value.y), static_cast<int>(value.z));
}

void Shader::setUniform(std::string_view name, glm::vec3ui value) const {
    glUniform3ui(glGetUniformLocation(this->handle, name.data()), value.x, value.y, value.z);
}

void Shader::setUniform(std::string_view name, glm::vec3i value) const {
    glUniform3i(glGetUniformLocation(this->handle, name.data()), value.x, value.y, value.z);
}

void Shader::setUniform(std::string_view name, glm::vec3f value) const {
    glUniform3f(glGetUniformLocation(this->handle, name.data()), value.x, value.y, value.z);
}

void Shader::setUniform(std::string_view name, glm::vec4b value) const {
    glUniform4i(glGetUniformLocation(this->handle, name.data()), static_cast<int>(value.x), static_cast<int>(value.y), static_cast<int>(value.z), static_cast<int>(value.w));
}

void Shader::setUniform(std::string_view name, glm::vec4ui value) const {
    glUniform4ui(glGetUniformLocation(this->handle, name.data()), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(std::string_view name, glm::vec4i value) const {
    glUniform4i(glGetUniformLocation(this->handle, name.data()), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(std::string_view name, glm::vec4f value) const {
    glUniform4f(glGetUniformLocation(this->handle, name.data()), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(std::string_view name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(this->handle, name.data()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::addPreprocessorSymbol(const std::string& name, const std::string& value) {
    if (!Shader::preprocessorSymbols.contains(name)) {
        Shader::preprocessorSymbols.insert(std::pair<std::string, std::string>{name, value});
    } else {
        Shader::preprocessorSymbols[name] = value;
    }
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
    for (std::sregex_iterator it{data.begin(), data.end(), includes}; it != std::sregex_iterator{}; it++) {
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

void Shader::addShader(const std::string& path, ShaderType type) {
    auto shaderString = Resource::getUniqueUncachedResource<StringResource>(path);
    auto shaderData = replaceMacros(shaderString->getIdentifier().data(), shaderString->getString());
    int shaderHandle = Renderer::createShader(shaderData, type);
    glAttachShader(this->handle, shaderHandle);
    this->shaderHandles[static_cast<int>(type)] = shaderHandle;
}

void Shader::setVertexShader(std::string path) {
    this->vertexPath = std::move(path);
    this->addShader(this->vertexPath, ShaderType::VERTEX);
}

void Shader::setFragmentShader(std::string path) {
    this->fragmentPath = std::move(path);
    this->addShader(this->fragmentPath, ShaderType::FRAGMENT);
}
