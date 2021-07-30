#pragma once

#include "shaderFile.h"
#include <glm/glm.hpp>
#include <functional>
#include <vector>
#include "../utility/handleObject.h"

class shader : public handleObject {
public:
    shader(const std::string& vert, const std::string& frag);
    virtual ~shader();
    void compile() override;
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
    void discard() override;
private:
    shaderFile vert;
    shaderFile frag;
    void checkForCompilationErrors() const;
};
