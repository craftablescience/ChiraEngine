#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../resource/abstractResource.h"
#include "light.h"
#include "shader.h"

class abstractMaterial {
public:
    explicit abstractMaterial(const std::string& shader_);
    virtual void use() = 0;
    virtual void compile() = 0;
    virtual shader* getShader();
protected:
    std::string shaderName;
};
