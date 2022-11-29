#pragma once

#include <loader/image/Image.h>
#include "ITexture.h"

namespace chira {

class Texture final : public ITexture {
public:
    explicit Texture(std::string identifier_, bool cacheTexture = true);
    ~Texture();
    void compile(const nlohmann::json& properties) override;
    void use() const override;
    void use(TextureUnit activeTextureUnit) const override;
protected:
    SharedPointer<Image> file;
    std::string filePath{"file://textures/missing.png"};
    WrapMode wrapModeS = WrapMode::REPEAT;
    std::string wrapModeSStr{"REPEAT"};
    WrapMode wrapModeT = WrapMode::REPEAT;
    std::string wrapModeTStr{"REPEAT"};
    FilterMode filterMode = FilterMode::LINEAR;
    std::string filterModeStr{"LINEAR"};
    bool mipmaps = true;
    bool verticalFlip = true;
    bool cache;
private:
    void setFilterMode(std::string filterModeStr_);
    void setWrapModeS(std::string wrapModeSStr_);
    void setWrapModeT(std::string wrapModeTStr_);
public:
    CHIRA_PROPS (
            CHIRA_PROP(Texture, verticalFlip),
            CHIRA_PROP(Texture, mipmaps),
            CHIRA_PROP_NAMED(Texture, filePath, image),
            CHIRA_PROP_NAMED_SET(Texture, wrapModeSStr, wrapModeS, setWrapModeS),
            CHIRA_PROP_NAMED_SET(Texture, wrapModeTStr, wrapModeT, setWrapModeT),
            CHIRA_PROP_NAMED_SET(Texture, filterModeStr, filterMode, setFilterMode)
    );
};

} // namespace chira
