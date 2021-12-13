#pragma once

#include <memory>
#include <string>
#include <glad/gl.h>
#include <resource/propertiesResource.h>
#include <loader/abstractImage.h>
#include <resource/textureResource.h>

namespace chira {
    class texture : public propertiesResource {
    public:
        explicit texture(const std::string& identifier_, bool cacheTexture = true);
        void compile(const nlohmann::json& properties) override;
        virtual void use() const;
        void setTextureUnit(int textureUnit);
        [[nodiscard]] int getTextureUnit() const;
        [[nodiscard]] unsigned int getHandle() const;
    protected:
        unsigned int handle = 0;
        int activeTextureUnit = -1;
        sharedPointer<textureResource> file;
        int format = GL_RGBA;
        int wrapModeS = GL_REPEAT;
        int wrapModeT = GL_REPEAT;
        int filterMode = GL_LINEAR;
        bool mipmaps = true;
        bool cache;
        static int getFormatFromString(const std::string& formatName);
        static int getWrapModeFromString(const std::string& wrapName);
        static int getFilterModeFromString(const std::string& filterName);
    };
}
