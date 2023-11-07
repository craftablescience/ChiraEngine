#pragma once

#include <concepts>
#include <string_view>

#include <core/math/Types.h>

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

template<typename T>
concept ShaderUniformValueTypes =
        std::same_as<T, bool>         ||
        std::same_as<T, unsigned int> ||
        std::same_as<T, int>          ||
        std::same_as<T, float>        ||

        std::same_as<T, glm::vec2b> ||
        std::same_as<T, glm::vec2u> ||
        std::same_as<T, glm::vec2i> ||
        std::same_as<T, glm::vec2f> ||

        std::same_as<T, glm::vec3b> ||
        std::same_as<T, glm::vec3u> ||
        std::same_as<T, glm::vec3i> ||
        std::same_as<T, glm::vec3f> ||

        std::same_as<T, glm::vec4b> ||
        std::same_as<T, glm::vec4u> ||
        std::same_as<T, glm::vec4i> ||
        std::same_as<T, glm::vec4f> ||
        std::same_as<T, glm::mat4>;

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
