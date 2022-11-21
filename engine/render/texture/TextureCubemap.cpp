#include "TextureCubemap.h"

#include <array>
#include <core/Logger.h>
#include <i18n/TranslationManager.h>
#include <render/backend/Renderer.h>
#include "Image.h"

using namespace chira;

CHIRA_CREATE_LOG(TEXTURECUBEMAP);

TextureCubemap::TextureCubemap(std::string identifier_)
    : ITexture(std::move(identifier_)) {}

void TextureCubemap::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);

    auto fileFD = Resource::getResource<Image>(this->imageFD, this->verticalFlipFD);
    auto fileBK = Resource::getResource<Image>(this->imageBK, this->verticalFlipBK);
    auto fileUP = Resource::getResource<Image>(this->imageUP, this->verticalFlipUP);
    auto fileDN = Resource::getResource<Image>(this->imageDN, this->verticalFlipDN);
    auto fileLT = Resource::getResource<Image>(this->imageLT, this->verticalFlipLT);
    auto fileRT = Resource::getResource<Image>(this->imageRT, this->verticalFlipRT);

    if (this->formatOverrideFD != "NONE") {
        this->formatFD = getTextureFormatFromString(this->formatOverrideFD);
    } else if (fileFD->getBitDepth() > 0) {
        this->formatFD = getTextureFormatFromBitDepth(fileFD->getBitDepth());
    }
    if (this->formatOverrideBK != "NONE") {
        this->formatBK = getTextureFormatFromString(this->formatOverrideBK);
    } else if (fileBK->getBitDepth() > 0) {
        this->formatBK = getTextureFormatFromBitDepth(fileBK->getBitDepth());
    }
    if (this->formatOverrideUP != "NONE") {
        this->formatUP = getTextureFormatFromString(this->formatOverrideUP);
    } else if (fileUP->getBitDepth() > 0) {
        this->formatUP = getTextureFormatFromBitDepth(fileUP->getBitDepth());
    }
    if (this->formatOverrideDN != "NONE") {
        this->formatDN = getTextureFormatFromString(this->formatOverrideDN);
    } else if (fileDN->getBitDepth() > 0) {
        this->formatDN = getTextureFormatFromBitDepth(fileDN->getBitDepth());
    }
    if (this->formatOverrideLT != "NONE") {
        this->formatLT = getTextureFormatFromString(this->formatOverrideLT);
    } else if (fileLT->getBitDepth() > 0) {
        this->formatLT = getTextureFormatFromBitDepth(fileLT->getBitDepth());
    }
    if (this->formatOverrideRT != "NONE") {
        this->formatRT = getTextureFormatFromString(this->formatOverrideRT);
    } else if (fileRT->getBitDepth() > 0) {
        this->formatRT = getTextureFormatFromBitDepth(fileRT->getBitDepth());
    }

    glGenTextures(1, &this->handle);

    if (this->activeTextureUnit == -1) {
        glActiveTexture(GL_TEXTURE0);
    } else {
        glActiveTexture(this->activeTextureUnit);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->handle);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, Renderer::getWrapMode(this->wrapModeS));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, Renderer::getWrapMode(this->wrapModeT));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, Renderer::getWrapMode(this->wrapModeR));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, Renderer::getFilterMode(this->filterMode));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, Renderer::getFilterMode(this->filterMode));

    std::array<Image*, 6> files{fileRT.get(), fileLT.get(), fileUP.get(), fileDN.get(), fileFD.get(), fileBK.get()};
    std::array<int, 6> formats{
        Renderer::getTextureFormat(this->formatRT),
        Renderer::getTextureFormat(this->formatLT),
        Renderer::getTextureFormat(this->formatUP),
        Renderer::getTextureFormat(this->formatDN),
        Renderer::getTextureFormat(this->formatFD),
        Renderer::getTextureFormat(this->formatBK),
    };
    for (int i = 0; i < 6; i++) {
        if (files[i]->getData()) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, formats[i], files[i]->getWidth(), files[i]->getHeight(), 0, formats[i], GL_UNSIGNED_BYTE, files[i]->getData());
        } else {
            LOG_TEXTURECUBEMAP.error(TRF("error.opengl.texture_cubemap_compile", i));
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
    this->filterMode = getFilterModeFromString(this->filterModeStr);
}

void TextureCubemap::setWrapModeS(std::string wrapModeSStr_) {
    this->wrapModeSStr = std::move(wrapModeSStr_);
    this->wrapModeS = getWrapModeFromString(this->wrapModeSStr);
}

void TextureCubemap::setWrapModeT(std::string wrapModeTStr_) {
    this->wrapModeTStr = std::move(wrapModeTStr_);
    this->wrapModeT = getWrapModeFromString(this->wrapModeTStr);
}

void TextureCubemap::setWrapModeR(std::string wrapModeRStr_) {
    this->wrapModeRStr = std::move(wrapModeRStr_);
    this->wrapModeR = getWrapModeFromString(this->wrapModeRStr);
}
