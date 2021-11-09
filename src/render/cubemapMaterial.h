#pragma once

#include "material.h"
#include "textureCubemap.h"

namespace chira {
    class cubemapMaterial : public material {
    public:
        explicit cubemapMaterial(const std::string& identifier_) : material(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        cubemapMaterial* copy() override;
        void use() override;
        void release() const override;
    private:
        textureCubemap* cubemap = nullptr;
    };
}
