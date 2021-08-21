#pragma once

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include "../resource/abstractMetaResource.h"
#include "shader.h"

class material : public abstractMetaResource {
public:
    material(const std::string& provider_, const std::string& name_);
    void compile(const nlohmann::json& properties) override;
    virtual void use();
    shader* getShader();
protected:
    shader* shaderPtr = nullptr;
    static int getGLFormatFromString(const std::string& formatName);
};
