#pragma once

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include "../resource/propertiesResource.h"
#include "shader.h"

class material : public propertiesResource {
public:
    material(const std::string& provider_, const std::string& name_);
    void compile(const nlohmann::json& properties) override;
    virtual void use();
    void release() const override;
    shader* getShader();
protected:
    shader* shaderPtr = nullptr;
    static int getGLFormatFromString(const std::string& formatName);
};
