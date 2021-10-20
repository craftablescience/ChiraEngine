#pragma once

#include <utility>
#include "material.h"
#include "texture.h"

namespace chira {
    class phongMaterial : public material {
    public:
        explicit phongMaterial(const std::string& identifier_) : material(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        void use() override;
        void release() const override;
        void setShininess(float shininess = 32.0f);
        void setLambertFactor(float lambertFactor = 1.0f);
    private:
        texture* diffuse = nullptr;
        texture* specular = nullptr;
    };
}
