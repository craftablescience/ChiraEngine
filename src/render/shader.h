#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <vector>
#include "../resource/shaderResource.h"
#include "../resource/abstractMetaResource.h"
#include "../utility/handleObject.h"

class shader : public abstractMetaResource, public handleObject {
public:
    shader(const std::string& provider_, const std::string& name_);
    void compile(const nlohmann::json& properties) override;
    ~shader() override;
    void use();
    void setUniform(const std::string& name, bool value) const;
    void setUniform(const std::string& name, unsigned int value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, bool value1, bool value2) const;
    void setUniform(const std::string& name, unsigned int value1, unsigned int value2) const;
    void setUniform(const std::string& name, int value1, int value2) const;
    void setUniform(const std::string& name, float value1, float value2) const;
    void setUniform(const std::string& name, bool value1, bool value2, bool value3) const;
    void setUniform(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3) const;
    void setUniform(const std::string& name, int value1, int value2, int value3) const;
    void setUniform(const std::string& name, float value1, float value2, float value3) const;
    void setUniform(const std::string& name, bool value1, bool value2, bool value3, bool value4) const;
    void setUniform(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4) const;
    void setUniform(const std::string& name, int value1, int value2, int value3, int value4) const;
    void setUniform(const std::string& name, float value1, float value2, float value3, float value4) const;
    void setUniform(const std::string& name, glm::mat4* value) const;
private:
    void checkForCompilationErrors() const;
    void discardInternal();
};
