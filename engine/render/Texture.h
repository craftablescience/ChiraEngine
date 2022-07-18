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
    int activeTextureUnit = -1;
    SharedPointer<TextureResource> file;
    int format = GL_RGBA;
    int wrapModeS = GL_REPEAT;
    int wrapModeT = GL_REPEAT;
    int filterMode = GL_LINEAR;
    bool mipmaps = true;
    bool cache;
    static int getFormatFromString(std::string_view formatName);
    static int getFormatFromBitDepth(int bd);
    static int getWrapModeFromString(std::string_view wrapName);
    static int getFilterModeFromString(std::string_view filterName);
};

}
