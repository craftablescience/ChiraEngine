#include "textureCubemap.h"

#include <resource/resource.h>
#include <i18n/translationManager.h>
#include <fmt/core.h>

using namespace chira;

textureCubemap::textureCubemap(const std::string& identifier_) : texture(identifier_) {}

void textureCubemap::compile(const nlohmann::json& properties) {
    this->wrapModeS = getWrapModeFromString(getPropertyOrDefault<std::string>(properties["properties"], "wrap_mode_s", "REPEAT"));
    this->wrapModeT = getWrapModeFromString(getPropertyOrDefault<std::string>(properties["properties"], "wrap_mode_t", "REPEAT"));
    this->wrapModeR = getWrapModeFromString(getPropertyOrDefault<std::string>(properties["properties"], "wrap_mode_r", "REPEAT"));
    this->filterMode = getFilterModeFromString(getPropertyOrDefault<std::string>(properties["properties"], "filter_mode", "LINEAR"));
    this->file = resource::getResource<textureResource>(properties["dependencies"]["image_fd"], getPropertyOrDefault<bool>(properties["properties"], "vertical_flip", false));
    this->file_bk = resource::getResource<textureResource>(properties["dependencies"]["image_bk"], getPropertyOrDefault<bool>(properties["properties"], "vertical_flip", false));
    this->file_up = resource::getResource<textureResource>(properties["dependencies"]["image_up"], getPropertyOrDefault<bool>(properties["properties"], "vertical_flip", false));
    this->file_dn = resource::getResource<textureResource>(properties["dependencies"]["image_dn"], getPropertyOrDefault<bool>(properties["properties"], "vertical_flip", false));
    this->file_lt = resource::getResource<textureResource>(properties["dependencies"]["image_lt"], getPropertyOrDefault<bool>(properties["properties"], "vertical_flip", false));
    this->file_rt = resource::getResource<textureResource>(properties["dependencies"]["image_rt"], getPropertyOrDefault<bool>(properties["properties"], "vertical_flip", false));
    this->format = getFormatFromString(getPropertyOrDefault<std::string>(properties["properties"], "format_fd", std::string("RGBA")));
    this->format_bk = getFormatFromString(getPropertyOrDefault<std::string>(properties["properties"], "format_bk", std::string("RGBA")));
    this->format_up = getFormatFromString(getPropertyOrDefault<std::string>(properties["properties"], "format_up", std::string("RGBA")));
    this->format_dn = getFormatFromString(getPropertyOrDefault<std::string>(properties["properties"], "format_dn", std::string("RGBA")));
    this->format_lt = getFormatFromString(getPropertyOrDefault<std::string>(properties["properties"], "format_lt", std::string("RGBA")));
    this->format_rt = getFormatFromString(getPropertyOrDefault<std::string>(properties["properties"], "format_rt", std::string("RGBA")));
    this->mipmaps = getPropertyOrDefault<bool>(properties["properties"], "mipmaps", false);

    if (this->handle != 0) return;
    glGenTextures(1, &this->handle);

    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->handle);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->wrapModeS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->wrapModeT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->wrapModeR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->filterMode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->filterMode);

    std::vector<textureResource*> files{this->file_rt.get(), this->file_lt.get(), this->file_up.get(), this->file_dn.get(), this->file.get(), this->file_bk.get()};
    std::vector<int> formats{this->format_rt, this->format_lt, this->format_up, this->format_dn, this->format, this->format_bk};
    for (int i = 0; i < 6; i++) {
        if (files[i]->getFile() && files[i]->getFile()->getData()) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, formats[i], files[i]->getWidth(), files[i]->getHeight(), 0, formats[i], GL_UNSIGNED_BYTE, files[i]->getData());
        } else {
            logger::log(ERR, "TextureCubemap", fmt::format(TR("error.opengl.texture_cubemap_compile"), i));
        }
    }
    if (this->mipmaps) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

void textureCubemap::use() const {
    if (this->handle == 0) return;
    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->handle);
}

sharedPointer<textureResource> textureCubemap::getTextureForward() const {
    return this->file;
}

sharedPointer<textureResource> textureCubemap::getTextureBackward() const {
    return this->file_bk;
}

sharedPointer<textureResource> textureCubemap::getTextureUp() const {
    return this->file_up;
}

sharedPointer<textureResource> textureCubemap::getTextureDown() const {
    return this->file_dn;
}

sharedPointer<textureResource> textureCubemap::getTextureLeft() const {
    return this->file_lt;
}

sharedPointer<textureResource> textureCubemap::getTextureRight() const {
    return this->file_rt;
}
