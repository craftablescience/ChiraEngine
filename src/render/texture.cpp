#include "texture.h"
#include "glad/gl.h"

texture::texture(const std::string& filepath) {
    int w, h, bd;
    this->file = new image(filepath, &w, &h, &bd, 0);
    this->width = w;
    this->height = h;
    this->bitDepth = bd;
    this->activeTextureUnit = -1;
}

texture::~texture() {
    if (this->handle != 0) delete this->file;
}

void texture::compile() {
    if (this->handle != 0) return;
    glGenTextures(1, &(this->handle));
}

void texture::discard() {
    if (this->handle != 0) delete this->file;
}

void texture::setTextureUnit(int textureUnit) {
    this->activeTextureUnit = textureUnit;
}

unsigned int texture::getHandle() const {
    return this->handle;
}
