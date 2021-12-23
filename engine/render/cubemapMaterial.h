#pragma once

#include "material.h"
#include "textureCubemap.h"

namespace chira {
    class cubemapMaterial : public material {
    public:
        explicit cubemapMaterial(const std::string& identifier_) : material(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        void use() override;
    private:
        sharedPointer<textureCubemap> cubemap;
    };
}
