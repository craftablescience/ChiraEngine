#include "textureResource.h"

#include "../loader/image.h"

using namespace chira;

textureResource::textureResource(const std::string& identifier_, bool vFlip_) : abstractResource(identifier_) {
    this->vFlip = vFlip_;
}

void textureResource::compile(const unsigned char buffer[], std::size_t bufferLen) {
    int w, h, bd;
    this->file = std::make_unique<image>(buffer, bufferLen - 1, &w, &h, &bd, 0, this->vFlip);
    this->width = w;
    this->height = h;
    this->bitDepth = bd;
}
