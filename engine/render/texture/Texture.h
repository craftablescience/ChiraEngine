#pragma once

#include <render/backend/RendererTypes.h>
#include "Image.h"
#include "ITexture.h"

namespace chira {

class Texture final : public ITexture {
public:
    explicit Texture(std::string identifier_, bool cacheTexture = true);
    void compile(const nlohmann::json& properties) override;
    void use() override;
protected:
    SharedPointer<Image> file;
    std::string filePath{"file://textures/missing.png"};
    TextureFormat format = TextureFormat::RGBA;
    std::string formatOverride{"NONE"};
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
            CHIRA_PROP(Texture, formatOverride),
            CHIRA_PROP(Texture, verticalFlip),
            CHIRA_PROP(Texture, mipmaps),
            CHIRA_PROP_NAMED(Texture, filePath, image),
            CHIRA_PROP_NAMED_SET(Texture, wrapModeSStr, wrapModeS, setWrapModeS),
            CHIRA_PROP_NAMED_SET(Texture, wrapModeTStr, wrapModeT, setWrapModeT),
            CHIRA_PROP_NAMED_SET(Texture, filterModeStr, filterMode, setFilterMode)
    );
};

} // namespace chira
