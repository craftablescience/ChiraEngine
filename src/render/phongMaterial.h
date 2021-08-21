#pragma once

#include <utility>

#include "material.h"
#include "texture2d.h"
#include "../core/engine.h"

class phongMaterial : public material {
public:
    phongMaterial(const std::string& provider_, const std::string& name_) : material(provider_, name_) {}
    void compile(const nlohmann::json& properties) override;
    void use() override;
    void release() const override;
    void setShininess(float shininess = 32.0f);
    void setLambertFactor(float lambertFactor = 1.0f);
private:
    texture2d* diffuse = nullptr;
    texture2d* specular = nullptr;
};
