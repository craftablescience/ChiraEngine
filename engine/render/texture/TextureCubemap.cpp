#include "TextureCubemap.h"

#include <core/Logger.h>
#include <loader/image/Image.h>
#include <render/backend/RenderBackend.h>

using namespace chira;

CHIRA_CREATE_LOG(TEXTURECUBEMAP);

TextureCubemap::TextureCubemap(std::string identifier_)
    : ITexture(std::move(identifier_)) {}

TextureCubemap::~TextureCubemap() {
    if (this->handle)
        Renderer::destroyTexture(this->handle);
}

void TextureCubemap::compile(const nlohmann::json& properties) {
    Reflect::fromJSON(this, properties);

    auto fileFD = Resource::getResource<Image>(this->imageFD, this->verticalFlipFD);
    auto fileBK = Resource::getResource<Image>(this->imageBK, this->verticalFlipBK);
    auto fileUP = Resource::getResource<Image>(this->imageUP, this->verticalFlipUP);
    auto fileDN = Resource::getResource<Image>(this->imageDN, this->verticalFlipDN);
    auto fileLT = Resource::getResource<Image>(this->imageLT, this->verticalFlipLT);
    auto fileRT = Resource::getResource<Image>(this->imageRT, this->verticalFlipRT);

    this->handle = Renderer::createTextureCubemap(*fileRT, *fileLT, *fileUP, *fileDN, *fileFD, *fileBK,
                                                  this->wrapModeS, this->wrapModeT, this->wrapModeR, this->filterMode,
                                                  this->mipmaps, TextureUnit::G0);
}

void TextureCubemap::use() const {
    Renderer::useTexture(this->handle, TextureUnit::G0);
}

void TextureCubemap::use(TextureUnit activeTextureUnit) const {
    Renderer::useTexture(this->handle, activeTextureUnit);
}

void TextureCubemap::setFilterMode(std::string filterModeStr_) {
    this->filterModeStr = std::move(filterModeStr_);
    this->filterMode = getFilterModeFromString(this->filterModeStr);
}

void TextureCubemap::setWrapModeS(std::string wrapModeSStr_) {
    this->wrapModeSStr = std::move(wrapModeSStr_);
    this->wrapModeS = getWrapModeFromString(this->wrapModeSStr);
}

void TextureCubemap::setWrapModeT(std::string wrapModeTStr_) {
    this->wrapModeTStr = std::move(wrapModeTStr_);
    this->wrapModeT = getWrapModeFromString(this->wrapModeTStr);
}

void TextureCubemap::setWrapModeR(std::string wrapModeRStr_) {
    this->wrapModeRStr = std::move(wrapModeRStr_);
    this->wrapModeR = getWrapModeFromString(this->wrapModeRStr);
}
