#pragma once

#include <render/texture/Texture.h>
#include "MaterialUntextured.h"

namespace chira {

class MaterialTextured final : public IMaterial {
public:
    explicit MaterialTextured(std::string path_) : IMaterial(std::move(path_)) {}
    void compile(const std::byte buffer[], std::size_t bufferLength) override;
    void use() const override;
    [[nodiscard]] SharedPointer<Texture> getTexture() const;
    void setTexture(std::string path);

protected:
    SharedPointer<Texture> texture;
    std::string texturePath{"textures/missing.json"};

public:
    template<typename Archive>
    void serialize(Archive& ar) {
        ar(
                cereal::make_nvp("shader", this->shaderPath),
                cereal::make_nvp("texture", this->texturePath)
        );
    }

private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialTextured);
};

} // namespace chira
