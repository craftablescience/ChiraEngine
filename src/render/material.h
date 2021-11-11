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
        std::shared_ptr<shader> getShader();
    protected:
        std::shared_ptr<shader> shaderPtr = nullptr;
    };
}
