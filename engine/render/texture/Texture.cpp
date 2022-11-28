#include "Texture.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

CHIRA_CREATE_LOG(TEXTURE);

Texture::Texture(std::string identifier_, bool cacheTexture)
    : ITexture(std::move(identifier_))
    , cache(cacheTexture) {}

void Texture::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);

    auto texData = Resource::getResource<Image>(this->filePath, this->verticalFlip);

    if (this->formatOverride != "NONE") {
        this->format = getFormatFromString(this->formatOverride);
    } else if (texData->getBitDepth() > 0) {
        this->format = getFormatFromBitDepth(texData->getBitDepth());
    }

    glGenTextures(1, &this->handle);

    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_2D, this->handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapModeT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMode);

    if (texData->getData()) {
        glTexImage2D(GL_TEXTURE_2D, 0, this->format, texData->getWidth(), texData->getHeight(), 0, this->format, GL_UNSIGNED_BYTE, texData->getData());
        if (this->mipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    } else {
        LOG_TEXTURE.error("Texture failed to compile: missing image data");
    }

    if (this->cache) {
        this->file = texData;
    }
}

void Texture::use() {
    if (this->handle == 0) return;
    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_2D, this->handle);
}

void Texture::setFilterMode(std::string filterModeStr_) {
    this->filterModeStr = std::move(filterModeStr_);
    this->filterMode = ITexture::getFilterModeFromString(this->filterModeStr);
}

void Texture::setWrapModeS(std::string wrapModeSStr_) {
    this->wrapModeSStr = std::move(wrapModeSStr_);
    this->wrapModeS = ITexture::getWrapModeFromString(this->wrapModeSStr);
}

void Texture::setWrapModeT(std::string wrapModeTStr_) {
    this->wrapModeTStr = std::move(wrapModeTStr_);
    this->wrapModeT = ITexture::getWrapModeFromString(this->wrapModeTStr);
}
