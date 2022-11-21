#include "Texture.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>
#include <render/backend/Renderer.h>

using namespace chira;

CHIRA_CREATE_LOG(TEXTURE);

Texture::Texture(std::string identifier_, bool cacheTexture)
    : ITexture(std::move(identifier_))
    , cache(cacheTexture) {}

void Texture::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);

    auto texData = Resource::getResource<Image>(this->filePath, this->verticalFlip);

    if (this->formatOverride != "NONE") {
        this->format = getTextureFormatFromString(this->formatOverride);
    } else if (texData->getBitDepth() > 0) {
        this->format = getTextureFormatFromBitDepth(texData->getBitDepth());
    }

    this->handle = Renderer::createTexture(this->format, this->wrapModeS, this->wrapModeT, this->filterMode, texData->getWidth(), texData->getHeight(), texData->getData(), true, this->activeTextureUnit);

    if (this->cache) {
        this->file = texData;
    }
}

void Texture::use() {
    Renderer::useTexture(this->handle, this->activeTextureUnit);
}

void Texture::setFilterMode(std::string filterModeStr_) {
    this->filterModeStr = std::move(filterModeStr_);
    this->filterMode = getFilterModeFromString(this->filterModeStr);
}

void Texture::setWrapModeS(std::string wrapModeSStr_) {
    this->wrapModeSStr = std::move(wrapModeSStr_);
    this->wrapModeS = getWrapModeFromString(this->wrapModeSStr);
}

void Texture::setWrapModeT(std::string wrapModeTStr_) {
    this->wrapModeTStr = std::move(wrapModeTStr_);
    this->wrapModeT = getWrapModeFromString(this->wrapModeTStr);
}
