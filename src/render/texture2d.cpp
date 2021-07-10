#include <iostream>
#include "texture2d.h"
#include "glad/gl.h"

texture2d::texture2d(const std::string& file, int format, bool mipmaps) : texture(file) {
    this->format = format;
    this->mipmaps = mipmaps;
}

void texture2d::compile() {
    if (this->handle != 0) return;
    texture::compile();
    glBindTexture(GL_TEXTURE_2D, this->handle);

    // todo: move repeat options to constructor
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // todo: move filtering options to constructor
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (this->file->getData()) {
        glTexImage2D(GL_TEXTURE_2D, 0, this->format, this->width, this->height, 0, this->format, GL_UNSIGNED_BYTE, this->file->getData());
        if (this->mipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    } else {
#if DEBUG
        std::cerr << "Error: texture failed to compile. Missing image data." << std::endl;
#endif
    }
}

void texture2d::use() {
    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_2D, this->handle);
}

void texture2d::discard() {
    texture::discard();
}
