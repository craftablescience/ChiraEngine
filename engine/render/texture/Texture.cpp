#include "Texture.h"

#include <core/Logger.h>
#include <render/backend/RenderBackend.h>

using namespace chira;

CHIRA_CREATE_LOG(TEXTURE);

Texture::Texture(std::string identifier_, bool cacheTexture /*= true*/)
    : ITexture(std::move(identifier_))
    , cache(cacheTexture) {}

Texture::~Texture() {
    if (this->handle)
        Renderer::destroyTexture(this->handle);
}

void Texture::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);

    auto imageFile = Resource::getResource<Image>(this->filePath, this->verticalFlip);

    this->handle = Renderer::createTexture2D(*imageFile, this->wrapModeS, this->wrapModeT, this->filterMode,
                                             this->mipmaps);
    if (this->cache) {
        this->file = imageFile;
    }
}

void Texture::use() const {
    Renderer::useTexture(this->handle);
}

void Texture::use(TextureUnit activeTextureUnit) const {
    Renderer::useTexture(this->handle, activeTextureUnit);
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
