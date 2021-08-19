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
    std::weak_ptr<shader> getShader();
protected:
    std::shared_ptr<shader> shaderPtr;
    static int getGLFormatFromString(const std::string& formatName);
};
