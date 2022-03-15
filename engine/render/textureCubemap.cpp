#include "textureCubemap.h"

#include <i18n/translationManager.h>
#include <utility/logger.h>

using namespace chira;

TextureCubemap::TextureCubemap(const std::string& identifier_) : Texture(identifier_, false) {}

void TextureCubemap::compile(const nlohmann::json& properties) {
    this->wrapModeS = getWrapModeFromString(getProperty<std::string>(properties["properties"], "wrap_mode_s", "REPEAT"));
    this->wrapModeT = getWrapModeFromString(getProperty<std::string>(properties["properties"], "wrap_mode_t", "REPEAT"));
    this->wrapModeR = getWrapModeFromString(getProperty<std::string>(properties["properties"], "wrap_mode_r", "REPEAT"));
    this->filterMode = getFilterModeFromString(getProperty<std::string>(properties["properties"], "filter_mode", "LINEAR"));

    auto file_fd = Resource::getResource<TextureResource>(
            getProperty<std::string>(properties["dependencies"], "image_fd", "file://textures/missing.png", true),
            getProperty(properties["properties"], "vertical_flip", false));
    auto file_bk = Resource::getResource<TextureResource>(
            getProperty<std::string>(properties["dependencies"], "image_bk", "file://textures/missing.png", true),
            getProperty(properties["properties"], "vertical_flip", false));
    auto file_up = Resource::getResource<TextureResource>(
            getProperty<std::string>(properties["dependencies"], "image_up", "file://textures/missing.png", true),
            getProperty(properties["properties"], "vertical_flip", false));
    auto file_dn = Resource::getResource<TextureResource>(
            getProperty<std::string>(properties["dependencies"], "image_dn", "file://textures/missing.png", true),
            getProperty(properties["properties"], "vertical_flip", false));
    auto file_lt = Resource::getResource<TextureResource>(
            getProperty<std::string>(properties["dependencies"], "image_lt", "file://textures/missing.png", true),
            getProperty(properties["properties"], "vertical_flip", false));
    auto file_rt = Resource::getResource<TextureResource>(
            getProperty<std::string>(properties["dependencies"], "image_rt", "file://textures/missing.png", true),
            getProperty(properties["properties"], "vertical_flip", false));

    if (hasProperty(properties["dependencies"], "image_fd"))
        this->format = getFormatFromString(getProperty<std::string>(properties["properties"], "format_fd", "RGB", true));
    else
        this->format = GL_RGB;
    if (hasProperty(properties["dependencies"], "image_bk"))
        this->format_bk = getFormatFromString(getProperty<std::string>(properties["properties"], "format_bk", "RGB", true));
    else
        this->format_bk = GL_RGB;
    if (hasProperty(properties["dependencies"], "image_up"))
        this->format_up = getFormatFromString(getProperty<std::string>(properties["properties"], "format_up", "RGB", true));
    else
        this->format_up = GL_RGB;
    if (hasProperty(properties["dependencies"], "image_dn"))
        this->format_dn = getFormatFromString(getProperty<std::string>(properties["properties"], "format_dn", "RGB", true));
    else
        this->format_dn = GL_RGB;
    if (hasProperty(properties["dependencies"], "image_lt"))
        this->format_lt = getFormatFromString(getProperty<std::string>(properties["properties"], "format_lt", "RGB", true));
    else
        this->format_lt = GL_RGB;
    if (hasProperty(properties["dependencies"], "image_rt"))
        this->format_rt = getFormatFromString(getProperty<std::string>(properties["properties"], "format_rt", "RGB", true));
    else
        this->format_rt = GL_RGB;
    this->mipmaps = getProperty(properties["properties"], "mipmaps", false);

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

    std::vector<TextureResource*> files{file_rt.get(), file_lt.get(), file_up.get(), file_dn.get(), file_fd.get(), file_bk.get()};
    std::vector<int> formats{this->format_rt, this->format_lt, this->format_up, this->format_dn, this->format, this->format_bk};
    for (int i = 0; i < 6; i++) {
        if (files[i]->getFile() && files[i]->getFile()->getData()) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, formats[i], files[i]->getWidth(), files[i]->getHeight(), 0, formats[i], GL_UNSIGNED_BYTE, files[i]->getData());
        } else {
            Logger::log(LogType::ERROR, "TextureCubemap", TRF("error.opengl.texture_cubemap_compile", i));
        }
    }
    if (this->mipmaps) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

void TextureCubemap::use() const {
    if (this->handle == 0)
        return;
    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->handle);
}
