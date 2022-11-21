#pragma once

#include <string_view>

namespace chira {

enum class TextureFormat {
    RED,
    RG,
    RGB,
    RGBA,
    BGR,
    BGRA,
    RED_INT,
    RG_INT,
    RGB_INT,
    RGBA_INT,
    BGR_INT,
    BGRA_INT,
    STENCIL,
    DEPTH,
    DEPTH_STENCIL,
};

enum class WrapMode {
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
};

enum class FilterMode {
    NEAREST,
    LINEAR,
};

TextureFormat getTextureFormatFromString(std::string_view format);
TextureFormat getTextureFormatFromBitDepth(int bd, bool flipRB = false, bool useInts = false);
WrapMode getWrapModeFromString(std::string_view mode);
FilterMode getFilterModeFromString(std::string_view mode);

} // namespace chira
