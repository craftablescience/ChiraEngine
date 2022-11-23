#pragma once

#include <string_view>

namespace chira {

enum class TextureType {
    TWO_DIMENSIONAL,
    CUBEMAP,
};

enum class TextureUnit : int {
    G0 = 0, G1, G2, G3, G4, G5, G6, G7, G8, G9, G10, G11, G12, G13, G14, G15,
};

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

[[nodiscard]] TextureFormat getTextureFormatFromString(std::string_view format);
[[nodiscard]] TextureFormat getTextureFormatFromBitDepth(int bd, bool flipRB = false, bool useInts = false);
[[nodiscard]] WrapMode getWrapModeFromString(std::string_view mode);
[[nodiscard]] FilterMode getFilterModeFromString(std::string_view mode);

enum class ShaderModuleType {
    VERTEX,
    FRAGMENT,
};

enum class MeshDrawMode {
    STATIC,
    DYNAMIC,
};

enum class MeshDepthFunction {
    NEVER,
    ALWAYS,
    EQUAL,
    NOTEQUAL,
    LESS,
    LEQUAL,
    GREATER,
    GEQUAL,
};

enum class MeshCullType {
    BACK,
    FRONT,
    NONE,
};

[[nodiscard]] MeshDepthFunction getMeshDepthFunctionFromString(std::string_view function);
[[nodiscard]] MeshCullType getMeshCullTypeFromString(std::string_view type);

} // namespace chira
