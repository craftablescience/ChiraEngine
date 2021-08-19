#include "texture2d.h"

texture2d::texture2d(const std::string& provider_, const std::string& name_, int format, int wrapModeU, int wrapModeV, int filterMode, bool mipmaps) : texture(provider_, name_) {
    this->format = format;
    this->wrapModeU = wrapModeU;
    this->wrapModeV = wrapModeV;
    this->filterMode = filterMode;
    this->mipmaps = mipmaps;
}

void texture2d::compile(std::unique_ptr<unsigned char> buffer, unsigned int bufferLen) {
    if (this->handle != 0) return;
    texture::compile(buffer.get(), bufferLen);
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
        // todo: use better logger in feat/wec-conversion
        //engine::logError("Texture2D", "Texture failed to compile. Missing image data");
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
