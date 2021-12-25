#pragma once

#include "untexturedMaterial.h"
#include "../texture.h"

namespace chira {
    class phongMaterial : public untexturedMaterial {
    public:
        explicit phongMaterial(const std::string& identifier_) : untexturedMaterial(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        void use() override;
        void setShininess(float shininess);
        void setLambertFactor(float lambertFactor);
    protected:
        sharedPointer<texture> diffuse;
        sharedPointer<texture> specular;
    private:
        REGISTER_PROPERTY_RESOURCE_TYPE(phongMaterial);
    };
}
