#include "TextureCubemap.h"

#include <core/Logger.h>
#include <i18n/TranslationManager.h>
#include <loader/image/Image.h>
#include <render/backend/Renderer.h>

using namespace chira;

CHIRA_CREATE_LOG(TEXTURECUBEMAP);

TextureCubemap::TextureCubemap(std::string identifier_)
    : ITexture(std::move(identifier_)) {}

void TextureCubemap::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);

    auto fileFD = Resource::getResource<Image>(this->imageFD, this->verticalFlipFD);
    auto fileBK = Resource::getResource<Image>(this->imageBK, this->verticalFlipBK);
    auto fileUP = Resource::getResource<Image>(this->imageUP, this->verticalFlipUP);
    auto fileDN = Resource::getResource<Image>(this->imageDN, this->verticalFlipDN);
    auto fileLT = Resource::getResource<Image>(this->imageLT, this->verticalFlipLT);
    auto fileRT = Resource::getResource<Image>(this->imageRT, this->verticalFlipRT);

    this->handle = Renderer::createTextureCubemap(*fileRT, *fileLT, *fileUP, *fileDN, *fileFD, *fileBK,
                                                  this->wrapModeS, this->wrapModeT, this->wrapModeR, this->filterMode,
                                                  this->mipmaps, this->activeTextureUnit);
}

void TextureCubemap::use() {
    Renderer::useTexture(TextureType::CUBEMAP, this->handle, this->activeTextureUnit);
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
