#pragma once

#include "texture.h"

namespace chira {
    class textureCubemap : public texture {
    public:
        explicit textureCubemap(const std::string& identifier_);
        void compile(const nlohmann::json& properties) override;
        textureCubemap* copy() override;
        void use() const override;
        ~textureCubemap() override;
        using texture::setTextureUnit;
        using texture::getTextureUnit;
        using texture::getHandle;
        [[nodiscard]] textureResource* getTextureForward() const;
        [[nodiscard]] textureResource* getTextureBackward() const;
        [[nodiscard]] textureResource* getTextureUp() const;
        [[nodiscard]] textureResource* getTextureDown() const;
        [[nodiscard]] textureResource* getTextureLeft() const;
        [[nodiscard]] textureResource* getTextureRight() const;
    private:
        // Disable this method
        using texture::getTexture;
        // Inherited variables from texture are used as file_fd, format_fd
        textureResource* file_bk = nullptr;
        textureResource* file_up = nullptr;
        textureResource* file_dn = nullptr;
        textureResource* file_lt = nullptr;
        textureResource* file_rt = nullptr;
        int format_bk = GL_RGBA;
        int format_up = GL_RGBA;
        int format_dn = GL_RGBA;
        int format_lt = GL_RGBA;
        int format_rt = GL_RGBA;
        int wrapModeR = GL_REPEAT;
    };
}
