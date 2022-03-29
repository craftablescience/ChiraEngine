#pragma once

#include <glm/glm.hpp>
#include <resource/propertiesResource.h>
#include <utility/handleObject.h>
#include <utility/math/types.h>

namespace chira {
    class Shader : public PropertiesResource, public HandleObject<int> {
    public:
        explicit Shader(const std::string& identifier_);
        void compile(const nlohmann::json& properties) override;
        ~Shader() override;
        void use() const;
        void setUniform(const std::string& name, bool value) const;
        void setUniform(const std::string& name, unsigned int value) const;
        void setUniform(const std::string& name, int value) const;
        void setUniform(const std::string& name, float value) const;
        void setUniform(const std::string& name, glm::vec2b value) const;
        void setUniform(const std::string& name, glm::vec2ui value) const;
        void setUniform(const std::string& name, glm::vec2i value) const;
        void setUniform(const std::string& name, glm::vec2f value) const;
        void setUniform(const std::string& name, glm::vec3b value) const;
        void setUniform(const std::string& name, glm::vec3ui value) const;
        void setUniform(const std::string& name, glm::vec3i value) const;
        void setUniform(const std::string& name, glm::vec3f value) const;
        void setUniform(const std::string& name, glm::vec4b value) const;
        void setUniform(const std::string& name, glm::vec4ui value) const;
        void setUniform(const std::string& name, glm::vec4i value) const;
        void setUniform(const std::string& name, glm::vec4f value) const;
        void setUniform(const std::string& name, glm::mat4 value) const;
        [[nodiscard]] bool usesModelMatrix() const {
            return this->usesModel;
        }
    private:
        /// Set when compiling
        bool usesModel = true;
        void checkForCompilationErrors() const;
    };
}
