#pragma once

#include "texture.h"

namespace chira {
    class textureCubemap : public texture {
    public:
        explicit textureCubemap(const std::string& identifier_);
        void compile(const nlohmann::json& properties) override;
        void use() const override;
        using texture::setTextureUnit;
        using texture::getTextureUnit;
        using texture::getHandle;
    private:
        // Inherited format, used as format_fd
        int format_bk = GL_RGBA;
        int format_up = GL_RGBA;
        int format_dn = GL_RGBA;
        int format_lt = GL_RGBA;
        int format_rt = GL_RGBA;
        int wrapModeR = GL_REPEAT;
    };
}
