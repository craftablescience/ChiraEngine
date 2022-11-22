#include "Texture.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>
#include <render/backend/Renderer.h>

using namespace chira;

CHIRA_CREATE_LOG(TEXTURE);

Texture::Texture(std::string identifier_, bool cacheTexture /*= true*/)
    : ITexture(std::move(identifier_))
    , cache(cacheTexture) {}

void Texture::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);

    auto imageFile = Resource::getResource<Image>(this->filePath, this->verticalFlip);

    this->handle = Renderer::createTexture2D(*imageFile, this->wrapModeS, this->wrapModeT, this->filterMode,
                                             this->mipmaps, this->activeTextureUnit);
    if (this->cache) {
        this->file = imageFile;
    }
}

void Texture::use() {
    Renderer::useTexture(TextureType::TWO_DIMENSIONAL, this->handle, this->activeTextureUnit);
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
