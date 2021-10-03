#include "texture2d.h"

#include "../utility/logger.h"
#include "../i18n/translationManager.h"

using namespace chira;

texture2d::texture2d(const std::string& identifier_, int format, bool vFlip_, int wrapModeU, int wrapModeV, int filterMode, bool mipmaps) : texture(identifier_, vFlip_) {
    this->format = format;
    this->wrapModeU = wrapModeU;
    this->wrapModeV = wrapModeV;
    this->filterMode = filterMode;
    this->mipmaps = mipmaps;
}

void texture2d::compile(unsigned char* buffer, std::size_t bufferLen) {
    texture::compile(buffer, bufferLen);
    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_2D, this->handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapModeU);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapModeV);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMode);

    if (this->file && this->file->getData()) {
        glTexImage2D(GL_TEXTURE_2D, 0, this->format, this->width, this->height, 0, this->format, GL_UNSIGNED_BYTE, this->file->getData());
        if (this->mipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    } else {
        logger::log(ERR, "Texture2D", TR("error.opengl.texture_compile"));
    }
}

void texture2d::use() {
    if (this->handle == 0) return;
    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_2D, this->handle);
}
