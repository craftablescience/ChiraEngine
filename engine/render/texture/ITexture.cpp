#include "ITexture.h"

#include <glad/gl.h>
#include <core/Logger.h>
#include <i18n/TranslationManager.h>

using namespace chira;

int ITexture::getFormatFromString(const std::string& formatName) {
    if (formatName == "RED")
        return GL_RED;
    else if (formatName == "RG")
        return GL_RG;
    else if (formatName == "RGB")
        return GL_RGB;
    else if (formatName == "BGR")
        return GL_BGR;
    else if (formatName == "RGBA")
        return GL_RGBA;
    else if (formatName == "BGRA")
        return GL_BGRA;
    else if (formatName == "RED_INT")
        return GL_RED_INTEGER;
    else if (formatName == "RG_INT")
        return GL_RG_INTEGER;
    else if (formatName == "RGB_INT")
        return GL_RGB_INTEGER;
    else if (formatName == "BGR_INT")
        return GL_BGR_INTEGER;
    else if (formatName == "RGBA_INT")
        return GL_RGBA_INTEGER;
    else if (formatName == "BGRA_INT")
        return GL_BGRA_INTEGER;
    else if (formatName == "STENCIL_INDEX")
        return GL_STENCIL_INDEX;
    else if (formatName == "DEPTH_COMPONENT")
        return GL_DEPTH_COMPONENT;
    else if (formatName == "DEPTH_STENCIL")
        return GL_DEPTH_STENCIL;

    Logger::log(LogType::LOG_WARNING, "Texture", TRF("warn.material.invalid_gl_format", formatName));
    return GL_RGBA;
}

int ITexture::getFormatFromBitDepth(int bd) {
    switch (bd) {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        default:
        case 4:
            return GL_RGBA;
    }
}

int ITexture::getWrapModeFromString(const std::string& wrapName) {
    if (wrapName == "REPEAT")
        return GL_REPEAT;
    else if (wrapName == "MIRRORED_REPEAT")
        return GL_MIRRORED_REPEAT;
    else if (wrapName == "CLAMP_TO_EDGE")
        return GL_CLAMP_TO_EDGE;
    else if (wrapName == "CLAMP_TO_BORDER")
        return GL_CLAMP_TO_BORDER;

    Logger::log(LogType::LOG_WARNING, "Texture", TRF("warn.material.invalid_gl_wrap_type", wrapName));
    return GL_REPEAT;
}

int ITexture::getFilterModeFromString(const std::string& filterName) {
    if (filterName == "NEAREST")
        return GL_NEAREST;
    else if (filterName == "LINEAR")
        return GL_LINEAR;
    // There are other filter types, but they only work on GL_TEXTURE_MIN_FILTER, so a refactor would be needed

    Logger::log(LogType::LOG_WARNING, "Texture", TRF("warn.material.invalid_gl_filter_type", filterName));
    return GL_LINEAR;
}
