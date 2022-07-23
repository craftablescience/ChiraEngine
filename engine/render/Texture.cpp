#include "Texture.h"

#include <core/Logger.h>
#include <loader/image/Image.h>
#include <i18n/TranslationManager.h>

using namespace chira;

Texture::Texture(std::string identifier_, bool cacheTexture)
    : PropertiesResource(std::move(identifier_))
    , HandleObject<unsigned int>(0)
    , cache(cacheTexture) {}

void Texture::compile(const nlohmann::json& properties) {
    this->wrapModeS = getWrapModeFromString(getProperty<std::string>(properties["properties"], "wrap_mode_s", "REPEAT"));
    this->wrapModeT = getWrapModeFromString(getProperty<std::string>(properties["properties"], "wrap_mode_t", "REPEAT"));
    this->filterMode = getFilterModeFromString(getProperty<std::string>(properties["properties"], "filter_mode", "LINEAR"));
    this->mipmaps = getProperty(properties["properties"], "mipmaps", true);

    auto texData = Resource::getResource<TextureResource>(
            getProperty<std::string>(properties["dependencies"], "image", "file://textures/missing.png", true),
            getProperty(properties["properties"], "vertical_flip", true));

    if (hasProperty(properties["properties"], "format_override"))
        this->format = getFormatFromString(properties["properties"]["format_override"]);
    else if (texData->getBitDepth() > 0)
        this->format = getFormatFromBitDepth(texData->getBitDepth());
    else
        this->format = GL_RGB;

    glGenTextures(1, &this->handle);

    if (this->activeTextureUnit == -1)
        glActiveTexture(GL_TEXTURE0);
    else
        glActiveTexture(this->activeTextureUnit);
    glBindTexture(GL_TEXTURE_2D, this->handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapModeT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMode);

    if (texData->getFile() && texData->getFile()->getData()) {
        glTexImage2D(GL_TEXTURE_2D, 0, this->format, texData->getWidth(), texData->getHeight(), 0, this->format, GL_UNSIGNED_BYTE, texData->getData());
        if (this->mipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        Logger::log(LOG_ERROR, "Texture", TR("error.opengl.texture_compile"));
    }

    if (this->cache)
        this->file = texData;
}

void Texture::use() const {
    if (this->handle == 0) return;
    if (this->activeTextureUnit == -1)
        glActiveTexture(GL_TEXTURE0);
    else
        glActiveTexture(this->activeTextureUnit);
    glBindTexture(GL_TEXTURE_2D, this->handle);
}

void Texture::setTextureUnit(int textureUnit) {
    this->activeTextureUnit = textureUnit;
}

int Texture::getTextureUnit() const {
    return this->activeTextureUnit;
}

int Texture::getFormatFromString(const std::string& formatName) {
    if (formatName == "RED")
        return GL_RED;
    else if (formatName == "RG")
        return GL_RG;
    else if (formatName == "RGB")
        return GL_RGB;
    else if (formatName == "BGR")
        return GL_BGR;
    else if (formatName == "RGBA")
        return GL_RGBA;
    else if (formatName == "BGRA")
        return GL_BGRA;
    else if (formatName == "RED_INT")
        return GL_RED_INTEGER;
    else if (formatName == "RG_INT")
        return GL_RG_INTEGER;
    else if (formatName == "RGB_INT")
        return GL_RGB_INTEGER;
    else if (formatName == "BGR_INT")
        return GL_BGR_INTEGER;
    else if (formatName == "RGBA_INT")
        return GL_RGBA_INTEGER;
    else if (formatName == "BGRA_INT")
        return GL_BGRA_INTEGER;
    else if (formatName == "STENCIL_INDEX")
        return GL_STENCIL_INDEX;
    else if (formatName == "DEPTH_COMPONENT")
        return GL_DEPTH_COMPONENT;
    else if (formatName == "DEPTH_STENCIL")
        return GL_DEPTH_STENCIL;

    Logger::log(LOG_WARNING, "Texture", TRF("warn.material.invalid_gl_format", formatName));
    return GL_RGBA;
}

int Texture::getFormatFromBitDepth(int bd) {
    switch (bd) {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        default: // covers bit depth of 4
            return GL_RGBA;
    }
}

int Texture::getWrapModeFromString(const std::string& wrapName) {
    if (wrapName == "REPEAT")
        return GL_REPEAT;
    else if (wrapName == "MIRRORED_REPEAT")
        return GL_MIRRORED_REPEAT;
    else if (wrapName == "CLAMP_TO_EDGE")
        return GL_CLAMP_TO_EDGE;
    else if (wrapName == "CLAMP_TO_BORDER")
        return GL_CLAMP_TO_BORDER;

    Logger::log(LOG_WARNING, "Texture", TRF("warn.material.invalid_gl_wrap_type", wrapName));
    return GL_REPEAT;
}

int Texture::getFilterModeFromString(const std::string& filterName) {
    if (filterName == "NEAREST")
        return GL_NEAREST;
    else if (filterName == "LINEAR")
        return GL_LINEAR;
    // There are other filter types, but they only work on GL_TEXTURE_MIN_FILTER, so a refactor would be needed

    Logger::log(LOG_WARNING, "Texture", TRF("warn.material.invalid_gl_filter_type", filterName));
    return GL_LINEAR;
}
