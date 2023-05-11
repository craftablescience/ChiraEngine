#pragma once

#include <render/texture/TextureCubemap.h>
#include "MaterialUntextured.h"

namespace chira {

class MaterialCubemap final : public IMaterial {
public:
    explicit MaterialCubemap(std::string identifier_) : IMaterial(std::move(identifier_)) {}
    void compile(const byte buffer[], std::size_t bufferLength) override;
    void use() const override;
    [[nodiscard]] SharedPointer<TextureCubemap> getTextureCubemap() const;
    void setTextureCubemap(std::string path);

protected:
    SharedPointer<TextureCubemap> cubemap;
    std::string cubemapPath{"file://textures/missing.json"};

public:
    template<typename Archive>
    void serialize(Archive& ar) {
        ar(
                cereal::make_nvp("shader", this->shaderPath),
                cereal::make_nvp("cubemap", this->cubemapPath)
        );
    }

private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialCubemap);
};

} // namespace chira
