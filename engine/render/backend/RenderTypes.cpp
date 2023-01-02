#include "RenderTypes.h"

#include <fmt/core.h>
#include <core/Logger.h>

using namespace chira;

CHIRA_CREATE_LOG(RENDERTYPES);

TextureFormat chira::getTextureFormatFromString(std::string_view format) {
    if (format == "RED") {
        return TextureFormat::RED;
    } else if (format == "RG") {
        return TextureFormat::RG;
    } else if (format == "RGB") {
        return TextureFormat::RGB;
    } else if (format == "RGBA") {
        return TextureFormat::RGBA;
    } else if (format == "BGR") {
        return TextureFormat::BGR;
    } else if (format == "BGRA") {
        return TextureFormat::BGRA;
    } else if (format == "RED_INT") {
        return TextureFormat::RED_INT;
    } else if (format == "RG_INT") {
        return TextureFormat::RG_INT;
    } else if (format == "RGB_INT") {
        return TextureFormat::RGB_INT;
    } else if (format == "RGBA_INT") {
        return TextureFormat::RGBA_INT;
    } else if (format == "BGR_INT") {
        return TextureFormat::BGR_INT;
    } else if (format == "BGRA_INT") {
        return TextureFormat::BGRA_INT;
    } else if (format == "STENCIL") {
        return TextureFormat::STENCIL;
    } else if (format == "DEPTH") {
        return TextureFormat::DEPTH;
    } else if (format == "DEPTH_STENCIL") {
        return TextureFormat::DEPTH_STENCIL;
    }

    LOG_RENDERTYPES.warning("Invalid string passed to getTextureFormatFromString: \"{}\"", format);
    return TextureFormat::RGBA;
}

TextureFormat chira::getTextureFormatFromBitDepth(int bd, bool flipRB /*= false*/, bool useInts /*= false*/) {
    if (!useInts) {
        if (!flipRB) {
            switch (bd) {
                case 1:
                    return TextureFormat::RED;
                case 2:
                    return TextureFormat::RG;
                case 3:
                    return TextureFormat::RGB;
                default:
                case 4:
                    return TextureFormat::RGBA;
            }
        } else {
            switch (bd) {
                case 1:
                    return TextureFormat::RED;
                case 2:
                    return TextureFormat::RG;
                case 3:
                    return TextureFormat::BGR;
                default:
                case 4:
                    return TextureFormat::BGRA;
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
                default:
                case 4:
                    return TextureFormat::RGBA_INT;
            }
        } else {
            switch (bd) {
                case 1:
                    return TextureFormat::RED_INT;
                case 2:
                    return TextureFormat::RG_INT;
                case 3:
                    return TextureFormat::BGR_INT;
                default:
                case 4:
                    return TextureFormat::BGRA_INT;
            }
        }
    }
}

WrapMode chira::getWrapModeFromString(std::string_view mode) {
    if (mode == "REPEAT") {
        return WrapMode::REPEAT;
    } else if (mode == "MIRRORED_REPEAT") {
        return WrapMode::MIRRORED_REPEAT;
    } else if (mode == "CLAMP_TO_BORDER") {
        return WrapMode::CLAMP_TO_BORDER;
    } else if (mode == "CLAMP_TO_EDGE") {
        return WrapMode::CLAMP_TO_EDGE;
    }

    LOG_RENDERTYPES.warning("Invalid string passed to getWrapModeFromString: \"{}\"", mode);
    return WrapMode::REPEAT;
}

FilterMode chira::getFilterModeFromString(std::string_view mode) {
    if (mode == "NEAREST") {
        return FilterMode::NEAREST;
    } else if (mode == "LINEAR") {
        return FilterMode::LINEAR;
    }

    LOG_RENDERTYPES.warning("Invalid string passed to getFilterModeFromString: \"{}\"", mode);
    return FilterMode::LINEAR;
}

MeshDepthFunction chira::getMeshDepthFunctionFromString(std::string_view function) {
    if (function == "NEVER")
        return MeshDepthFunction::NEVER;
    else if (function == "ALWAYS")
        return MeshDepthFunction::ALWAYS;
    else if (function == "LESS")
        return MeshDepthFunction::LESS;
    else if (function == "LEQUAL")
        return MeshDepthFunction::LEQUAL;
    else if (function == "EQUAL")
        return MeshDepthFunction::EQUAL;
    else if (function == "GEQUAL")
        return MeshDepthFunction::GEQUAL;
    else if (function == "GREATER")
        return MeshDepthFunction::GREATER;
    else if (function == "NOTEQUAL")
        return MeshDepthFunction::NOTEQUAL;

    LOG_RENDERTYPES.warning("Invalid string passed to getMeshDepthFunctionFromString: \"{}\"", function);
    return MeshDepthFunction::LEQUAL;
}

MeshCullType chira::getMeshCullTypeFromString(std::string_view type) {
    if (type == "BACK")
        return MeshCullType::BACK;
    else if (type == "FRONT")
        return MeshCullType::FRONT;
    else if (type == "NONE")
        return MeshCullType::NONE;

    LOG_RENDERTYPES.warning("Invalid string passed to getMeshCullTypeFromString: \"{}\"", type);
    return MeshCullType::BACK;
}
