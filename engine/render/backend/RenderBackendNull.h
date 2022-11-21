#pragma once

#include <string_view>
#include <math/Types.h>
#include "RendererTypes.h"

namespace chira {

struct RenderBackendNull {
    RenderBackendNull() = delete;

    [[nodiscard]] static inline std::string_view getHumanName() { return "Null"; }
    [[nodiscard]] static inline bool setupForDebugging() { return true; }

    [[nodiscard]] static inline int getTextureFormat(TextureFormat /*format*/) { return 0; }
    [[nodiscard]] static inline int getTextureFormatFromBitDepth(int /*bd*/) { return 0; }
    [[nodiscard]] static inline int getWrapMode(WrapMode /*mode*/) { return 0; }
    [[nodiscard]] static inline int getFilterMode(FilterMode /*mode*/) { return 0; }

    [[nodiscard]] static inline unsigned int createTexture(TextureFormat /*format*/, WrapMode /*wrapS*/, WrapMode /*wrapT*/, FilterMode /*filter*/, int /*width*/, int /*height*/, byte /*data*/[], bool /*genMipmaps*/ = true, int /*activeTextureUnit*/ = -1) { return 0; }
    static inline void useTexture(unsigned int /*handle*/, int /*activeTextureUnit*/ = -1) {}
};

} // namespace chira
