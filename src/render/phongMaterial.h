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
        ~phongMaterial() override;
        void setShininess(float shininess = 32.0f);
        void setLambertFactor(float lambertFactor = 1.0f);
    private:
        std::shared_ptr<texture> diffuse = nullptr;
        std::shared_ptr<texture> specular = nullptr;
    };
}
