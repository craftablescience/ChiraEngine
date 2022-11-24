#pragma once

#include <string_view>
#include <vector>
#include <loader/image/Image.h>
#include <math/Types.h>
#include <math/Vertex.h>
#include "RendererTypes.h"

namespace chira {

struct RenderBackendGL {
    RenderBackendGL() = delete;

    struct TextureHandle {
        unsigned int handle;
        TextureType type;
    };

    struct ShaderModuleHandle {
        int handle;
    };

    struct MeshHandle {
        unsigned int vaoHandle;
        unsigned int vboHandle;
        unsigned int eboHandle;
    };

    [[nodiscard]] static std::string_view getHumanName();
    [[nodiscard]] static bool setupForDebugging();

    [[nodiscard]] static TextureHandle createTexture2D(const Image& image, WrapMode wrapS, WrapMode wrapT, FilterMode filter,
                                                       bool genMipmaps = true, TextureUnit activeTextureUnit = TextureUnit::G0);
    [[nodiscard]] static TextureHandle createTextureCubemap(const Image& imageRT, const Image& imageLT, const Image& imageUP,
                                                            const Image& imageDN, const Image& imageFD, const Image& imageBK,
                                                            WrapMode wrapS, WrapMode wrapT, WrapMode wrapR, FilterMode filter,
                                                            bool genMipmaps = true, TextureUnit activeTextureUnit = TextureUnit::G0);
    static void useTexture(TextureHandle handle, TextureUnit activeTextureUnit = TextureUnit::G0);

    [[nodiscard]] static ShaderModuleHandle createShaderModule(const std::string& shader, ShaderModuleType type);
    static void destroyShaderModule(ShaderModuleHandle handle);

    [[nodiscard]] static MeshHandle createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode);
    static void updateMesh(MeshHandle handle, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode);
    static void drawMesh(MeshHandle handle, const std::vector<Index>& indices, MeshDepthFunction depthFunction, MeshCullType cullType);
    static void destroyMesh(MeshHandle handle);
};

} // namespace chira
