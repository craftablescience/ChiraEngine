#pragma once

#include <render/texture/Texture.h>
#include "MaterialUntextured.h"

namespace chira {

class MaterialPhong final : public IMaterial {
public:
    explicit MaterialPhong(std::string identifier_) : IMaterial(std::move(identifier_)) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
    [[nodiscard]] SharedPointer<Texture> getTextureDiffuse() const;
    void setTextureDiffuse(std::string path);
    [[nodiscard]] SharedPointer<Texture> getTextureSpecular() const;
    void setTextureSpecular(std::string path);
    [[nodiscard]] float getShininess() const;
    void setShininess(float shininess);
    [[nodiscard]] float getLambertFactor() const;
    void setLambertFactor(float lambertFactor);
protected:
    SharedPointer<Texture> diffuse;
    std::string diffusePath{"file://textures/missing.json"};
    SharedPointer<Texture> specular;
    std::string specularPath{"file://textures/missing.json"};
    float shininess = 32.f;
    float lambertFactor = 1.f;
public:
    CHIRA_PROPS_INHERITED(IMaterial) (
            CHIRA_PROP_NAMED_SET(MaterialPhong, diffusePath, diffuse, setTextureDiffuse),
            CHIRA_PROP_NAMED_SET(MaterialPhong, specularPath, specular, setTextureSpecular),
            CHIRA_PROP_SET(MaterialPhong, shininess, setShininess),
            CHIRA_PROP_SET(MaterialPhong, lambertFactor, setLambertFactor)
    );
private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialPhong);
};

} // namespace chira
