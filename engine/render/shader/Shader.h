#pragma once

#include <glm/glm.hpp>
#include <math/Types.h>
#include <render/backend/Renderer.h>
#include <resource/PropertiesResource.h>

namespace chira {

constexpr std::string_view SHADER_PREPROCESSOR_DEFAULT_PREFIX = "#";
constexpr std::string_view SHADER_PREPROCESSOR_DEFAULT_SUFFIX = "#";

class Shader : public PropertiesResource {
    // todo(render): UBO needs this to get the shader handle
    friend class UniformBufferObject;
public:
    explicit Shader(std::string identifier_);
    void compile(const nlohmann::json& properties) override;
    void use() const;
    ~Shader() override;

    void setUniform(std::string_view name, bool value) const;
    void setUniform(std::string_view name, unsigned int value) const;
    void setUniform(std::string_view name, int value) const;
    void setUniform(std::string_view name, float value) const;
    void setUniform(std::string_view name, glm::vec2b value) const;
    void setUniform(std::string_view name, glm::vec2ui value) const;
    void setUniform(std::string_view name, glm::vec2i value) const;
    void setUniform(std::string_view name, glm::vec2f value) const;
    void setUniform(std::string_view name, glm::vec3b value) const;
    void setUniform(std::string_view name, glm::vec3ui value) const;
    void setUniform(std::string_view name, glm::vec3i value) const;
    void setUniform(std::string_view name, glm::vec3f value) const;
    void setUniform(std::string_view name, glm::vec4b value) const;
    void setUniform(std::string_view name, glm::vec4ui value) const;
    void setUniform(std::string_view name, glm::vec4i value) const;
    void setUniform(std::string_view name, glm::vec4f value) const;
    void setUniform(std::string_view name, glm::mat4 value) const;

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

    int handle = -1;
    Renderer::ShaderModuleHandle shaderModuleHandles[2] {{-1}, {-1}};
    bool usesPV = true;
    bool usesM = true;
    bool lit = true;
    std::string vertexPath{"file://shaders/unlitTextured.vsh"};
    std::string fragmentPath{"file://shaders/unlitTextured.fsh"};

    /// Should only be called in Shader::compile() !!!
    void addShaderModule(const std::string& path, ShaderModuleType type);
    /// Should only be called in Shader::compile() !!!
    void setVertexShader(std::string path);
    /// Should only be called in Shader::compile() !!!
    void setFragmentShader(std::string path);
public:
    CHIRA_PROPS(
            CHIRA_PROP(Shader, usesPV),
            CHIRA_PROP(Shader, usesM),
            CHIRA_PROP(Shader, lit),
            CHIRA_PROP_NAMED_SET(Shader, vertexPath, vertex, setVertexShader),
            CHIRA_PROP_NAMED_SET(Shader, fragmentPath, fragment, setFragmentShader)
    );
};

} // namespace chira
