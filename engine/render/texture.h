#pragma once

#include <glad/gl.h>
#include <resource/propertiesResource.h>
#include <resource/textureResource.h>
#include <utility/handleObject.h>

namespace chira {
    class Texture : public PropertiesResource, public HandleObject<unsigned int> {
    public:
        explicit Texture(const std::string& identifier_, bool cacheTexture = true);
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
        static int getFormatFromString(const std::string& formatName);
        static int getFormatFromBitDepth(int bd);
        static int getWrapModeFromString(const std::string& wrapName);
        static int getFilterModeFromString(const std::string& filterName);
    };
}
