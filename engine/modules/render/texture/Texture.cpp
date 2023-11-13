#include "Texture.h"

#include <core/debug/Logger.h>
#include <render/backend/RenderBackend.h>

using namespace chira;

CHIRA_CREATE_LOG(TEXTURE);

Texture::Texture(std::string path_, bool cacheTexture /*= true*/)
    : ITexture(std::move(path_))
    , cache(cacheTexture) {}

Texture::~Texture() {
    if (this->handle)
        Renderer::destroyTexture(this->handle);
}

void Texture::compile(const std::byte buffer[], std::size_t bufferLength) {
    Serial::loadFromBuffer(this, buffer, bufferLength);

    auto imageFile = Resource::getResource<Image>(this->filePath, this->verticalFlip);

    this->handle = Renderer::createTexture2D(*imageFile, this->wrapModeS, this->wrapModeT, this->filterMode,
                                             this->mipmaps, TextureUnit::G0);
    if (this->cache) {
        this->file = imageFile;
    }
}

void Texture::use() const {
    Renderer::useTexture(this->handle, TextureUnit::G0);
}

void Texture::use(TextureUnit activeTextureUnit) const {
    Renderer::useTexture(this->handle, activeTextureUnit);
}
