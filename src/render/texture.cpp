#include "texture.h"
#include "glad/gl.h"
#include "../core/virtualFileSystem.h"

texture::texture(const std::string& filepath) {
    int w, h, bd;
    this->file.reset(new image(virtualFileSystem::getTexturePath(filepath), &w, &h, &bd, 0));
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
