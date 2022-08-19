#pragma once

#include <glm/glm.hpp>
#include <resource/PropertiesResource.h>
#include <math/Types.h>
#include <utility/HandleObject.h>

namespace chira {

class Shader : public PropertiesResource, public HandleObject<int> {
public:
    explicit Shader(std::string identifier_);
    void compile(const nlohmann::json& properties) override;
    ~Shader() override;
    void use() const;
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
    [[nodiscard]] bool usesPVMatrices() const {
        return this->usesPV;
    }
    [[nodiscard]] bool usesModelMatrix() const {
        return this->usesM;
    }
    [[nodiscard]] bool isLit() const {
        return this->lit;
    }
private:
    void checkForCompilationErrors() const;

    bool usesPV = true;
    bool usesM = true;
    bool lit = true;
    std::string vertexPath{"file://shaders/unlitTextured.vsh"};
    std::string fragmentPath{"file://shaders/unlitTextured.fsh"};
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
