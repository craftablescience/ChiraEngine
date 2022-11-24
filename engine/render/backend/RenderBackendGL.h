#pragma once

#include <string_view>
#include <vector>
#include <loader/image/Image.h>
#include <math/Types.h>
#include <math/Vertex.h>
#include "RendererTypes.h"

/// OpenGL 4.1 (or 4.3) render backend
namespace chira::Renderer {

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

[[nodiscard]] std::string_view getHumanName();
[[nodiscard]] bool setupForDebugging();

[[nodiscard]] TextureHandle createTexture2D(const Image& image, WrapMode wrapS, WrapMode wrapT, FilterMode filter,
                                            bool genMipmaps = true, TextureUnit activeTextureUnit = TextureUnit::G0);
[[nodiscard]] TextureHandle createTextureCubemap(const Image& imageRT, const Image& imageLT, const Image& imageUP,
                                                 const Image& imageDN, const Image& imageFD, const Image& imageBK,
                                                 WrapMode wrapS, WrapMode wrapT, WrapMode wrapR, FilterMode filter,
                                                 bool genMipmaps = true, TextureUnit activeTextureUnit = TextureUnit::G0);
void useTexture(TextureHandle handle, TextureUnit activeTextureUnit = TextureUnit::G0);

[[nodiscard]] ShaderModuleHandle createShaderModule(const std::string& shader, ShaderModuleType type);
void destroyShaderModule(ShaderModuleHandle handle);

[[nodiscard]] MeshHandle createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode);
void updateMesh(MeshHandle handle, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode);
void drawMesh(MeshHandle handle, const std::vector<Index>& indices, MeshDepthFunction depthFunction, MeshCullType cullType);
void destroyMesh(MeshHandle handle);

} // namespace chira::Renderer
