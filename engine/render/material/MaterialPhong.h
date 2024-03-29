#pragma once

#include <render/texture/Texture.h>
#include "MaterialUntextured.h"

namespace chira {

class MaterialPhong final : public IMaterial {
public:
    explicit MaterialPhong(std::string identifier_) : IMaterial(std::move(identifier_)) {}
    void compile(const byte buffer[], std::size_t bufferLength) override;
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
    template<typename Archive>
    void serialize(Archive& ar) {
        ar(
                cereal::make_nvp("shader", this->shaderPath),
                cereal::make_nvp("diffuse", this->diffusePath),
                cereal::make_nvp("specular", this->specularPath),
                cereal::make_nvp("shininess", this->shininess),
                cereal::make_nvp("lambertFactor", this->lambertFactor)
        );
    }

private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialPhong);
};

} // namespace chira
