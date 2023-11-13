#include "TextureCubemap.h"

#include <core/debug/Logger.h>
#include <loader/image/Image.h>
#include <render/backend/RenderBackend.h>

using namespace chira;

CHIRA_CREATE_LOG(TEXTURECUBEMAP);

TextureCubemap::TextureCubemap(std::string path_)
    : ITexture(std::move(path_)) {}

TextureCubemap::~TextureCubemap() {
    if (this->handle)
        Renderer::destroyTexture(this->handle);
}

void TextureCubemap::compile(const std::byte buffer[], std::size_t bufferLength) {
    Serial::loadFromBuffer(this, buffer, bufferLength);

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
