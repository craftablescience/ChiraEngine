#pragma once

#include <string_view>
#include <math/Types.h>
#include "RendererTypes.h"

namespace chira {

struct RenderBackendGL {
    RenderBackendGL() = delete;

    [[nodiscard]] static std::string_view getHumanName();
    [[nodiscard]] static bool setupForDebugging();

    [[nodiscard]] static int getTextureFormat(TextureFormat format);
    [[nodiscard]] static int getTextureFormatFromBitDepth(int bd);
    [[nodiscard]] static int getWrapMode(WrapMode mode);
    [[nodiscard]] static int getFilterMode(FilterMode mode);

    [[nodiscard]] static unsigned int createTexture(TextureFormat format, WrapMode wrapS, WrapMode wrapT, FilterMode filter, int width, int height, byte data[], bool genMipmaps = true, int activeTextureUnit = -1);
    static void useTexture(unsigned int handle, int activeTextureUnit = -1);
};

} // namespace chira
