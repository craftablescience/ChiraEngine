#include "TextureCubemap.h"

#include <array>
#include <core/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

TextureCubemap::TextureCubemap(std::string identifier_)
    : ITexture(std::move(identifier_)) {}

void TextureCubemap::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);

    auto fileFD = Resource::getResource<TextureResource>(this->imageFD, this->verticalFlipFD);
    auto fileBK = Resource::getResource<TextureResource>(this->imageBK, this->verticalFlipBK);
    auto fileUP = Resource::getResource<TextureResource>(this->imageUP, this->verticalFlipUP);
    auto fileDN = Resource::getResource<TextureResource>(this->imageDN, this->verticalFlipDN);
    auto fileLT = Resource::getResource<TextureResource>(this->imageLT, this->verticalFlipLT);
    auto fileRT = Resource::getResource<TextureResource>(this->imageRT, this->verticalFlipRT);

    if (this->formatOverrideFD != "NONE") {
        this->formatFD = getFormatFromString(this->formatOverrideFD);
    } else if (fileFD->getBitDepth() > 0) {
        this->formatFD = getFormatFromBitDepth(fileFD->getBitDepth());
    }
    if (this->formatOverrideBK != "NONE") {
        this->formatBK = getFormatFromString(this->formatOverrideBK);
    } else if (fileBK->getBitDepth() > 0) {
        this->formatBK = getFormatFromBitDepth(fileBK->getBitDepth());
    }
    if (this->formatOverrideUP != "NONE") {
        this->formatUP = getFormatFromString(this->formatOverrideUP);
    } else if (fileUP->getBitDepth() > 0) {
        this->formatUP = getFormatFromBitDepth(fileUP->getBitDepth());
    }
    if (this->formatOverrideDN != "NONE") {
        this->formatDN = getFormatFromString(this->formatOverrideDN);
    } else if (fileDN->getBitDepth() > 0) {
        this->formatDN = getFormatFromBitDepth(fileDN->getBitDepth());
    }
    if (this->formatOverrideLT != "NONE") {
        this->formatLT = getFormatFromString(this->formatOverrideLT);
    } else if (fileLT->getBitDepth() > 0) {
        this->formatLT = getFormatFromBitDepth(fileLT->getBitDepth());
    }
    if (this->formatOverrideRT != "NONE") {
        this->formatRT = getFormatFromString(this->formatOverrideRT);
    } else if (fileRT->getBitDepth() > 0) {
        this->formatRT = getFormatFromBitDepth(fileRT->getBitDepth());
    }

    glGenTextures(1, &this->handle);

    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->handle);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->wrapModeS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->wrapModeT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->wrapModeR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->filterMode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->filterMode);

    std::array<TextureResource*, 6> files{fileRT.get(), fileLT.get(), fileUP.get(), fileDN.get(), fileFD.get(), fileBK.get()};
    std::array<int, 6> formats{this->formatRT, this->formatLT, this->formatUP, this->formatDN, this->formatFD, this->formatBK};
    for (int i = 0; i < 6; i++) {
        if (files[i]->getFile() && files[i]->getFile()->getData()) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, formats[i], files[i]->getWidth(), files[i]->getHeight(), 0, formats[i], GL_UNSIGNED_BYTE, files[i]->getData());
        } else {
            Logger::log(LogType::LOG_ERROR, "TextureCubemap", TRF("error.opengl.texture_cubemap_compile", i));
        }
    }
    if (this->mipmaps) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

void TextureCubemap::use() {
    if (this->handle == 0)
        return;
    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->handle);
}

void TextureCubemap::setFilterMode(std::string filterModeStr_) {
    this->filterModeStr = std::move(filterModeStr_);
    this->filterMode = ITexture::getFilterModeFromString(this->filterModeStr);
}

void TextureCubemap::setWrapModeS(std::string wrapModeSStr_) {
    this->wrapModeSStr = std::move(wrapModeSStr_);
    this->wrapModeS = ITexture::getWrapModeFromString(this->wrapModeSStr);
}

void TextureCubemap::setWrapModeT(std::string wrapModeTStr_) {
    this->wrapModeTStr = std::move(wrapModeTStr_);
    this->wrapModeT = ITexture::getWrapModeFromString(this->wrapModeTStr);
}

void TextureCubemap::setWrapModeR(std::string wrapModeRStr_) {
    this->wrapModeRStr = std::move(wrapModeRStr_);
    this->wrapModeR = ITexture::getWrapModeFromString(this->wrapModeRStr);
}
