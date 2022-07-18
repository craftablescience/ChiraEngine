#pragma once

#include "MaterialUntextured.h"
#include "../Texture.h"

namespace chira {

class MaterialPhong : public MaterialUntextured {
public:
    explicit MaterialPhong(std::string identifier_) : MaterialUntextured(std::move(identifier_)) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
    [[nodiscard]] SharedPointer<Texture> getTextureDiffuse() const;
    [[nodiscard]] SharedPointer<Texture> getTextureSpecular() const;
    void setShininess(float shininess);
    void setLambertFactor(float lambertFactor);
protected:
    SharedPointer<Texture> diffuse;
    SharedPointer<Texture> specular;
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialPhong); // NOLINT(cert-err58-cpp)
};

}
