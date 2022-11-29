#pragma once

#include <string_view>
#include <vector>
#include <loader/image/Image.h>
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

struct ShaderHandle {
    int handle;
    ShaderModuleHandle vertex;
    ShaderModuleHandle fragment;
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

[[nodiscard]] ShaderHandle createShader(std::string_view vertex, std::string_view fragment);
void useShader(ShaderHandle handle);
void destroyShader(ShaderHandle handle);

void setShaderUniform(ShaderHandle handle, std::string_view name, bool value);
void setShaderUniform(ShaderHandle handle, std::string_view name, unsigned int value);
void setShaderUniform(ShaderHandle handle, std::string_view name, int value);
void setShaderUniform(ShaderHandle handle, std::string_view name, float value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec2b value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec2ui value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec2i value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec2f value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec3b value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec3ui value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec3i value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec3f value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec4b value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec4ui value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec4i value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::vec4f value);
void setShaderUniform(ShaderHandle handle, std::string_view name, glm::mat4 value);

[[nodiscard]] MeshHandle createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode);
void updateMesh(MeshHandle handle, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode);
void drawMesh(MeshHandle handle, const std::vector<Index>& indices, MeshDepthFunction depthFunction, MeshCullType cullType);
void destroyMesh(MeshHandle handle);

} // namespace chira::Renderer
