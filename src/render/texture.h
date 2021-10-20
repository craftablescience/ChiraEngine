#pragma once

#include <memory>
#include <string>
#include <glad/gl.h>
#include "../resource/propertiesResource.h"
#include "../loader/abstractImage.h"
#include "../resource/textureResource.h"

namespace chira {
    class texture : public propertiesResource {
    public:
        explicit texture(const std::string& identifier_);
        void compile(const nlohmann::json& properties) override;
        void use() const;
        ~texture() override;
        [[nodiscard]] textureResource* getTexture() const;
        void setTextureUnit(int textureUnit);
        [[nodiscard]] int getTextureUnit() const;
        [[nodiscard]] unsigned int getHandle() const;
    protected:
        unsigned int handle = 0;
        int activeTextureUnit = -1;
        textureResource* file = nullptr;
        int format = GL_RGBA;
        int wrapModeU = GL_REPEAT;
        int wrapModeV = GL_REPEAT;
        int filterMode = GL_LINEAR;
        bool mipmaps = true;
    private:
        static int getFormatFromString(const std::string& formatName);
        static int getWrapModeFromString(const std::string& wrapName);
        static int getFilterModeFromString(const std::string& filterName);
    };
}
