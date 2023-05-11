#pragma once

#include <render/backend/RenderTypes.h>
#include <utility/Serial.h>
#include "ITexture.h"

namespace chira {

class TextureCubemap final : public ITexture {
public:
    explicit TextureCubemap(std::string identifier_);
    ~TextureCubemap() override;
    void compile(const byte buffer[], std::size_t bufferLength) override;
    void use() const override;
    void use(TextureUnit activeTextureUnit) const override;

protected:
    std::string imageFD{"file://textures/missing.png"};
    std::string imageBK{"file://textures/missing.png"};
    std::string imageUP{"file://textures/missing.png"};
    std::string imageDN{"file://textures/missing.png"};
    std::string imageLT{"file://textures/missing.png"};
    std::string imageRT{"file://textures/missing.png"};
    bool verticalFlipFD = false;
    bool verticalFlipBK = false;
    bool verticalFlipUP = false;
    bool verticalFlipDN = false;
    bool verticalFlipLT = false;
    bool verticalFlipRT = false;
    bool mipmaps = true;
    FilterMode filterMode = FilterMode::LINEAR;
    WrapMode wrapModeS = WrapMode::REPEAT;
    WrapMode wrapModeT = WrapMode::REPEAT;
    WrapMode wrapModeR = WrapMode::REPEAT;

public:
    template<typename Archive>
    void serialize(Archive& ar) {
        ar(
                cereal::make_nvp("verticalFlipFD", this->verticalFlipFD),
                cereal::make_nvp("verticalFlipBK", this->verticalFlipBK),
                cereal::make_nvp("verticalFlipUP", this->verticalFlipUP),
                cereal::make_nvp("verticalFlipDN", this->verticalFlipDN),
                cereal::make_nvp("verticalFlipLT", this->verticalFlipLT),
                cereal::make_nvp("verticalFlipRT", this->verticalFlipRT),
                cereal::make_nvp("mipmaps", this->mipmaps),
                cereal::make_nvp("imageFD", this->imageFD),
                cereal::make_nvp("imageBK", this->imageBK),
                cereal::make_nvp("imageUP", this->imageUP),
                cereal::make_nvp("imageDN", this->imageDN),
                cereal::make_nvp("imageLT", this->imageLT),
                cereal::make_nvp("imageRT", this->imageRT),
                cereal::make_nvp("wrapModeS", this->wrapModeS),
                cereal::make_nvp("wrapModeT", this->wrapModeT),
                cereal::make_nvp("wrapModeR", this->wrapModeR),
                cereal::make_nvp("filterMode", this->filterMode)
        );
    }
};

} // namespace chira
