#pragma once

#include <string>
#include "../core/engine.h"

class abstractMaterial {
public:
    explicit abstractMaterial(const std::string& shader_);
    virtual void use() = 0;
    virtual shader* getShader();
protected:
    std::string shader;
};
