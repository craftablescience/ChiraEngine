#pragma once

#include <glm/glm.hpp>
#include <math/Types.h>
#include <render/backend/RenderBackend.h>
#include <resource/PropertiesResource.h>

namespace chira {

constexpr std::string_view SHADER_PREPROCESSOR_DEFAULT_PREFIX = "#";
constexpr std::string_view SHADER_PREPROCESSOR_DEFAULT_SUFFIX = "#";

class Shader : public PropertiesResource {
public:
    explicit Shader(std::string identifier_);
    void compile(const nlohmann::json& properties) override;
    void use() const;
    ~Shader() override;

    inline void setUniform(std::string_view name, bool value) {
        Renderer::setShaderUniform1b(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, unsigned int value) {
        Renderer::setShaderUniform1u(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, int value) {
        Renderer::setShaderUniform1i(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, float value) {
        Renderer::setShaderUniform1f(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec2b value) {
        Renderer::setShaderUniform2b(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec2u value) {
        Renderer::setShaderUniform2u(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec2i value) {
        Renderer::setShaderUniform2i(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec2f value) {
        Renderer::setShaderUniform2f(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec3b value) {
        Renderer::setShaderUniform3b(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec3u value) {
        Renderer::setShaderUniform3u(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec3i value) {
        Renderer::setShaderUniform3i(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec3f value) {
        Renderer::setShaderUniform3f(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec4b value) {
        Renderer::setShaderUniform4b(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec4u value) {
        Renderer::setShaderUniform4u(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec4i value) {
        Renderer::setShaderUniform4i(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::vec4f value) {
        Renderer::setShaderUniform4f(this->handle, name, value);
    }
    inline void setUniform(std::string_view name, glm::mat4 value) {
        Renderer::setShaderUniform4m(this->handle, name, value);
    }

    [[nodiscard]] inline bool usesPVMatrices() const {
        return this->usesPV;
    }
    [[nodiscard]] inline bool usesModelMatrix() const {
        return this->usesM;
    }
    [[nodiscard]] inline bool isLit() const {
        return this->lit;
    }

    static void addPreprocessorSymbol(const std::string& name, const std::string& value);
    static void setPreprocessorPrefix(const std::string& prefix);
    static void setPreprocessorSuffix(const std::string& suffix);
private:
    static inline std::unordered_map<std::string, std::string> preprocessorSymbols;
    static inline std::string preprocessorPrefix = std::string{SHADER_PREPROCESSOR_DEFAULT_PREFIX};
    static inline std::string preprocessorSuffix = std::string{SHADER_PREPROCESSOR_DEFAULT_SUFFIX};

    static std::string replaceMacros(const std::string&, const std::string&);

    Renderer::ShaderHandle handle{};
    bool usesPV = true;
    bool usesM = true;
    bool lit = true;
    std::string vertexPath{"file://shaders/unlitTextured.vsh"};
    std::string fragmentPath{"file://shaders/unlitTextured.fsh"};
public:
    CHIRA_PROPS() (
            CHIRA_PROP(Shader, usesPV),
            CHIRA_PROP(Shader, usesM),
            CHIRA_PROP(Shader, lit),
            CHIRA_PROP_NAMED(Shader, vertexPath, vertex),
            CHIRA_PROP_NAMED(Shader, fragmentPath, fragment)
    );
};

} // namespace chira
