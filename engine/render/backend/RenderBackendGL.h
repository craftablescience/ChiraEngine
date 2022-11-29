#pragma once

#include <cstddef>
#include <string_view>
#include <vector>
#include <loader/image/Image.h>
#include <math/Vertex.h>
#include "RendererTypes.h"

/// OpenGL 4.1 (or 4.3) render backend
namespace chira::Renderer {

struct TextureHandle {
    unsigned int handle = 0;
    TextureType type = TextureType::TWO_DIMENSIONAL;

    explicit inline operator bool() const { return handle; }
    inline bool operator!() const { return !handle; }
};

struct ShaderModuleHandle {
    int handle = 0;

    explicit inline operator bool() const { return handle; }
    inline bool operator!() const { return !handle; }
};

struct ShaderHandle {
    int handle = 0;
    ShaderModuleHandle vertex{};
    ShaderModuleHandle fragment{};

    explicit inline operator bool() const { return handle && vertex && fragment; }
    inline bool operator!() const { return !handle || !vertex || !fragment; }
};

struct UniformBufferHandle {
    unsigned int handle = 0;
    unsigned int bindingPoint = 0;

    explicit inline operator bool() const { return handle; }
    inline bool operator!() const { return handle; }
};

struct MeshHandle {
    unsigned int vaoHandle = 0;
    unsigned int vboHandle = 0;
    unsigned int eboHandle = 0;

    explicit inline operator bool() const { return vaoHandle && vboHandle && eboHandle; }
    inline bool operator!() const { return !vaoHandle || !vboHandle || !eboHandle; }
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
void destroyTexture(TextureHandle handle);

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

[[nodiscard]] UniformBufferHandle createUniformBuffer(std::ptrdiff_t size);
void bindUniformBufferToShader(ShaderHandle shaderHandle, UniformBufferHandle uniformBufferHandle, std::string_view name);
void updateUniformBuffer(UniformBufferHandle handle, const void* buffer, std::ptrdiff_t length);
void updateUniformBufferPart(UniformBufferHandle handle, std::ptrdiff_t start, const void* buffer, std::ptrdiff_t length);
void destroyUniformBuffer(UniformBufferHandle handle);

[[nodiscard]] MeshHandle createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode);
void updateMesh(MeshHandle handle, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode);
void drawMesh(MeshHandle handle, const std::vector<Index>& indices, MeshDepthFunction depthFunction, MeshCullType cullType);
void destroyMesh(MeshHandle handle);

} // namespace chira::Renderer
