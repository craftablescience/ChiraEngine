#include "textureResource.h"

#include <loader/image/image.h>

using namespace chira;

TextureResource::TextureResource(const std::string& identifier_, bool vFlip_) : Resource(identifier_) {
    this->vFlip = vFlip_;
}

void TextureResource::compile(const unsigned char buffer[], std::size_t bufferLen) {
    int w, h, bd;
    this->file = std::make_unique<Image>(buffer, bufferLen - 1, &w, &h, &bd, 0, this->vFlip);
    this->width = w;
    this->height = h;
    this->bitDepth = bd;
}
