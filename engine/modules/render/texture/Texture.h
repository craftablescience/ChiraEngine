#pragma once

#include <core/utility/Serial.h>
#include <loader/image/Image.h>
#include "ITexture.h"

namespace chira {

class Texture final : public ITexture {
public:
    explicit Texture(std::string path_, bool cacheTexture = true);
    ~Texture() override;
    void compile(const std::byte buffer[], std::size_t bufferLength) override;
    void use() const override;
    void use(TextureUnit activeTextureUnit) const override;

protected:
    SharedPointer<Image> file;
    std::string filePath{"textures/missing.png"};
    WrapMode wrapModeS = WrapMode::REPEAT;
    WrapMode wrapModeT = WrapMode::REPEAT;
    FilterMode filterMode = FilterMode::LINEAR;
    bool mipmaps = true;
    bool verticalFlip = true;
    bool cache;

public:
    template<typename Archive>
    void serialize(Archive& ar) {
        ar(
                cereal::make_nvp("verticalFlip", this->verticalFlip),
                cereal::make_nvp("mipmaps", this->mipmaps),
                cereal::make_nvp("image", this->filePath),
                cereal::make_nvp("wrapModeS", this->wrapModeS),
                cereal::make_nvp("wrapModeT", this->wrapModeT),
                cereal::make_nvp("filterMode", this->filterMode)
        );
    }
};

} // namespace chira
