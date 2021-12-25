#pragma once

#include "untexturedMaterial.h"
#include "../textureCubemap.h"

namespace chira {
    class cubemapMaterial : public untexturedMaterial {
    public:
        explicit cubemapMaterial(const std::string& identifier_) : untexturedMaterial(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        void use() override;
    private:
        sharedPointer<textureCubemap> cubemap;
    };
}
