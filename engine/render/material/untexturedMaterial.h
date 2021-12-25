#pragma once

#include <resource/propertiesResource.h>
#include "../shader.h"

namespace chira {
    class untexturedMaterial : public propertiesResource {
    public:
        explicit untexturedMaterial(const std::string& identifier_);
        void compile(const nlohmann::json& properties) override;
        virtual void use();
        sharedPointer<shader> getShader();
    protected:
        sharedPointer<shader> shaderPtr;
    };
}
