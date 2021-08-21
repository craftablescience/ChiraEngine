#include "texture.h"

#include <glad/gl.h>
#include "../loader/image.h"

texture::texture(const std::string& provider_, const std::string& name_) : abstractResource(provider_, name_) {}

void texture::compile(unsigned char* buffer, std::size_t bufferLen) {
    int w, h, bd;
    this->file = std::make_unique<image>(buffer, bufferLen - 1, &w, &h, &bd);
    this->width = w;
    this->height = h;
    this->bitDepth = bd;
    if (this->handle != 0) return;
    glGenTextures(1, &this->handle);
}

void texture::setTextureUnit(int textureUnit) {
    this->activeTextureUnit = textureUnit;
}

int texture::getTextureUnit() const {
    return this->activeTextureUnit;
}

unsigned int texture::getHandle() const {
    return this->handle;
}
