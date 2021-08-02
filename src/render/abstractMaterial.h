#pragma once

#include <string>
#include <vector>
#include <memory>
#include "light.h"
#include "shader.h"

class abstractMaterial {
public:
    explicit abstractMaterial(const std::string& shader_);
    virtual void use() = 0;
    virtual void compile() = 0;
    virtual void updateLighting(const std::vector<std::unique_ptr<abstractLight>>& lights) = 0;
    virtual shader* getShader();
protected:
    std::string shaderName;
};
