#pragma once

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include "../resource/propertiesResource.h"
#include "shader.h"

namespace chira {
    class material : public propertiesResource {
    public:
        explicit material(const std::string& identifier_);
        void compile(const nlohmann::json& properties) override;
        virtual void use();
        void release() const override;
        shader* getShader();
    protected:
        shader* shaderPtr = nullptr;
    };
}
