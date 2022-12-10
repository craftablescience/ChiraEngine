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

    inline void setUniform(std::string_view name, ShaderUniformValueTypes auto value) {
        Renderer::setShaderUniform(this->handle, name, value);
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
    static inline std::string preprocessorPrefix = std::string{SHADER_PREPROCESSOR_DEFAULT_PREFIX}; // NOLINT(cert-err58-cpp)
    static inline std::string preprocessorSuffix = std::string{SHADER_PREPROCESSOR_DEFAULT_SUFFIX}; // NOLINT(cert-err58-cpp)

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
