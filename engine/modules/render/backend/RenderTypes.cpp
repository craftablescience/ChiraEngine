#include "RenderTypes.h"

#include <fmt/core.h>
#include <magic_enum.hpp>

#include <core/debug/Assertions.h>
#include <core/debug/Logger.h>

using namespace chira;

CHIRA_CREATE_LOG(RENDERTYPES);

TextureFormat chira::getTextureFormatFromString(std::string_view format) {
    if (auto value = magic_enum::enum_cast<TextureFormat>(format)) {
        return *value;
    }
    LOG_RENDERTYPES.warning("Invalid string passed to getTextureFormatFromString: \"{}\"", format);
    return TextureFormat::RGBA;
}

TextureFormat chira::getTextureFormatFromBitDepth(int bd, bool flipRB /*= false*/, bool useInts /*= false*/) {
    chiraAssert(bd >= 1 && bd <= 4, "Bit depth is not between 1 and 4 inclusive!");
    if (bd > 4) bd = 4;
    else if (bd < 1) bd = 1;

    if (!useInts) {
        if (!flipRB) {
            switch (bd) {
                case 1:
                    return TextureFormat::RED;
                case 2:
                    return TextureFormat::RG;
                case 3:
                    return TextureFormat::RGB;
                case 4:
                    return TextureFormat::RGBA;
                CHIRA_NO_DEFAULT;
            }
        } else {
            switch (bd) {
                case 1:
                    return TextureFormat::RED;
                case 2:
                    return TextureFormat::RG;
                case 3:
                    return TextureFormat::BGR;
                case 4:
                    return TextureFormat::BGRA;
                CHIRA_NO_DEFAULT;
            }
        }
    } else {
        if (!flipRB) {
            switch (bd) {
                case 1:
                    return TextureFormat::RED_INT;
                case 2:
                    return TextureFormat::RG_INT;
                case 3:
                    return TextureFormat::RGB_INT;
                case 4:
                    return TextureFormat::RGBA_INT;
                CHIRA_NO_DEFAULT;
            }
        } else {
            switch (bd) {
                case 1:
                    return TextureFormat::RED_INT;
                case 2:
                    return TextureFormat::RG_INT;
                case 3:
                    return TextureFormat::BGR_INT;
                case 4:
                    return TextureFormat::BGRA_INT;
                CHIRA_NO_DEFAULT;
            }
        }
    }
}

WrapMode chira::getWrapModeFromString(std::string_view mode) {
    if (auto value = magic_enum::enum_cast<WrapMode>(mode)) {
        return *value;
    }
    LOG_RENDERTYPES.warning("Invalid string passed to getWrapModeFromString: \"{}\"", mode);
    return WrapMode::REPEAT;
}

FilterMode chira::getFilterModeFromString(std::string_view mode) {
    if (auto value = magic_enum::enum_cast<FilterMode>(mode)) {
        return *value;
    }
    LOG_RENDERTYPES.warning("Invalid string passed to getFilterModeFromString: \"{}\"", mode);
    return FilterMode::LINEAR;
}

MeshDepthFunction chira::getMeshDepthFunctionFromString(std::string_view function) {
    if (auto value = magic_enum::enum_cast<MeshDepthFunction>(function)) {
        return *value;
    }
    LOG_RENDERTYPES.warning("Invalid string passed to getMeshDepthFunctionFromString: \"{}\"", function);
    return MeshDepthFunction::LEQUAL;
}

MeshCullType chira::getMeshCullTypeFromString(std::string_view type) {
    if (auto value = magic_enum::enum_cast<MeshCullType>(type)) {
        return *value;
    }
    LOG_RENDERTYPES.warning("Invalid string passed to getMeshCullTypeFromString: \"{}\"", type);
    return MeshCullType::BACK;
}
