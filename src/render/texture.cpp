#include "texture.h"

#include "glad/gl.h"
#include "../loader/image.h"
#include "../core/virtualFileSystem.h"

texture::texture(const std::string& filepath) {
    int w, h, bd;
    this->file = std::make_unique<image>(virtualFileSystem::getTexturePath(filepath), &w, &h, &bd, 0);
    this->width = w;
    this->height = h;
    this->bitDepth = bd;
}

texture::texture(abstractImage* image, int w, int h, int bd) {
    this->file.reset(image);
    this->width = w;
    this->height = h;
    this->bitDepth = bd;
}

void texture::compile() {
    if (this->handle != 0) return;
    glGenTextures(1, &(this->handle));
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
