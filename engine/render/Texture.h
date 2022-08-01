#pragma once

#include <glad/gl.h>
#include <resource/PropertiesResource.h>
#include <resource/TextureResource.h>
#include <utility/HandleObject.h>

namespace chira {

class Texture : public PropertiesResource, public HandleObject<unsigned int> {
public:
    explicit Texture(std::string identifier_, bool cacheTexture = true);
    void compile(const nlohmann::json& properties) override;
    virtual void use() const;
    void setTextureUnit(int textureUnit);
    [[nodiscard]] int getTextureUnit() const;
protected:
    static int getFormatFromString(const std::string& formatName);
    static int getFormatFromBitDepth(int bd);
    static int getWrapModeFromString(const std::string& wrapName);
    static int getFilterModeFromString(const std::string& filterName);

    int activeTextureUnit = -1;
    SharedPointer<TextureResource> file;
    std::string filePath{"file://textures/missing.png"};
    int format = GL_RGBA;
    std::string formatOverride{"NONE"};
    int wrapModeS = GL_REPEAT;
    std::string wrapModeSStr{"REPEAT"};
    int wrapModeT = GL_REPEAT;
    std::string wrapModeTStr{"REPEAT"};
    int filterMode = GL_LINEAR;
    std::string filterModeStr{"LINEAR"};
    bool mipmaps = true;
    bool verticalFlip = true;
    bool cache;
private:
    void setFilePath(std::string path);
    void setWrapModeS(std::string wrapModeSStr_);
    void setWrapModeT(std::string wrapModeTStr_);
    void setFilterMode(std::string filterModeStr_);
public:
    CHIRA_PROPS (
            CHIRA_PROP(Texture, formatOverride),
            CHIRA_PROP(Texture, verticalFlip),
            CHIRA_PROP(Texture, mipmaps),
            CHIRA_PROP_NAMED_SET(Texture, filePath, image, setFilePath),
            CHIRA_PROP_NAMED_SET(Texture, wrapModeSStr, wrapModeS, setWrapModeS),
            CHIRA_PROP_NAMED_SET(Texture, wrapModeTStr, wrapModeT, setWrapModeT),
            CHIRA_PROP_NAMED_SET(Texture, filterModeStr, filterMode, setFilterMode)
    );
};

} // namespace chira
