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
        [[nodiscard]] std::shared_ptr<textureResource> getTextureForward() const;
        [[nodiscard]] std::shared_ptr<textureResource> getTextureBackward() const;
        [[nodiscard]] std::shared_ptr<textureResource> getTextureUp() const;
        [[nodiscard]] std::shared_ptr<textureResource> getTextureDown() const;
        [[nodiscard]] std::shared_ptr<textureResource> getTextureLeft() const;
        [[nodiscard]] std::shared_ptr<textureResource> getTextureRight() const;
    private:
        // Disable this method
        using texture::getTexture;
        // Inherited variables from texture are used as file_fd, format_fd
        std::shared_ptr<textureResource> file_bk = nullptr;
        std::shared_ptr<textureResource> file_up = nullptr;
        std::shared_ptr<textureResource> file_dn = nullptr;
        std::shared_ptr<textureResource> file_lt = nullptr;
        std::shared_ptr<textureResource> file_rt = nullptr;
        int format_bk = GL_RGBA;
        int format_up = GL_RGBA;
        int format_dn = GL_RGBA;
        int format_lt = GL_RGBA;
        int format_rt = GL_RGBA;
        int wrapModeR = GL_REPEAT;
    };
}
