#pragma once

#include "texture.h"

namespace chira {
    class textureCubemap : public texture {
    public:
        explicit textureCubemap(const std::string& identifier_);
        void compile(const nlohmann::json& properties) override;
        void use() const override;
        ~textureCubemap() override;
        using texture::setTextureUnit;
        using texture::getTextureUnit;
        using texture::getHandle;
        [[nodiscard]] sharedPointer<textureResource> getTextureForward() const;
        [[nodiscard]] sharedPointer<textureResource> getTextureBackward() const;
        [[nodiscard]] sharedPointer<textureResource> getTextureUp() const;
        [[nodiscard]] sharedPointer<textureResource> getTextureDown() const;
        [[nodiscard]] sharedPointer<textureResource> getTextureLeft() const;
        [[nodiscard]] sharedPointer<textureResource> getTextureRight() const;
    private:
        // Disable this method
        using texture::getTexture;
        // Inherited variables from texture are used as file_fd, format_fd
        sharedPointer<textureResource> file_bk;
        sharedPointer<textureResource> file_up;
        sharedPointer<textureResource> file_dn;
        sharedPointer<textureResource> file_lt;
        sharedPointer<textureResource> file_rt;
        int format_bk = GL_RGBA;
        int format_up = GL_RGBA;
        int format_dn = GL_RGBA;
        int format_lt = GL_RGBA;
        int format_rt = GL_RGBA;
        int wrapModeR = GL_REPEAT;
    };
}
