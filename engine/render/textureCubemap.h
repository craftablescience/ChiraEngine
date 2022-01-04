#pragma once

#include "texture.h"

namespace chira {
    class TextureCubemap : public Texture {
    public:
        explicit TextureCubemap(const std::string& identifier_);
        void compile(const nlohmann::json& properties) override;
        void use() const override;
        using Texture::setTextureUnit;
        using Texture::getTextureUnit;
        using Texture::getHandle;
    protected:
        // Inherited format, used as format_fd
        int format_bk = GL_RGBA;
        int format_up = GL_RGBA;
        int format_dn = GL_RGBA;
        int format_lt = GL_RGBA;
        int format_rt = GL_RGBA;
        int wrapModeR = GL_REPEAT;
    };
}
