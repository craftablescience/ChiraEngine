#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

#include <core/math/Color.h>
#include <core/math/Vertex.h>
#include <loader/image/Image.h>
#include "../RenderTypes.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

/// OpenGL 4.1 (or 4.3) render backend
namespace chira::Renderer {

struct TextureHandle {
    unsigned int handle = 0;
    SDL_Texture* texture = nullptr;

    TextureType type = TextureType::TWO_DIMENSIONAL;

    explicit inline operator bool() const { return handle; }
    inline bool operator!() const { return !handle; }
};

struct FrameBufferHandle {
    SDL_Texture* texture = nullptr;
    unsigned int colorHandle = 0;

    bool hasDepth = true;
    int width = -1;
    int height = -1;

    explicit inline operator bool() const { return texture; }
    inline bool operator!() const { return !texture; }
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
    inline bool operator!() const { return !handle; }
};

struct MeshHandle {
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    int numIndices = 0;
    int numVertices = 0;

    explicit inline operator bool() const { return numIndices; }
    inline bool operator!() const { return !numIndices; }
};

[[nodiscard]] std::string_view getHumanName();
[[nodiscard]] bool setupForDebugging();

void setClearColor(ColorRGBA color);

[[nodiscard]] TextureHandle createTexture2D(const Image& image, WrapMode wrapS, WrapMode wrapT, FilterMode filter,
                                            bool genMipmaps, TextureUnit activeTextureUnit);
[[nodiscard]] TextureHandle createTextureCubemap(const Image& imageRT, const Image& imageLT, const Image& imageUP,
                                                 const Image& imageDN, const Image& imageFD, const Image& imageBK,
                                                 WrapMode wrapS, WrapMode wrapT, WrapMode wrapR, FilterMode filter,
                                                 bool genMipmaps, TextureUnit activeTextureUnit);
void useTexture(TextureHandle handle, TextureUnit activeTextureUnit);
[[nodiscard]] void* getImGuiTextureHandle(TextureHandle handle);
void destroyTexture(TextureHandle handle);

[[nodiscard]] FrameBufferHandle createFrameBuffer(int width, int height, WrapMode wrapS, WrapMode wrapT, FilterMode filter, bool hasDepth);
void recreateFrameBuffer(Renderer::FrameBufferHandle* handle, int width, int height, WrapMode wrapS, WrapMode wrapT, FilterMode filter, bool hasDepth);
void pushFrameBuffer(FrameBufferHandle handle);
void popFrameBuffer();
void useFrameBufferTexture(const FrameBufferHandle handle, TextureUnit activeTextureUnit);
[[nodiscard]] void* getImGuiFrameBufferHandle(FrameBufferHandle handle);
void destroyFrameBuffer(FrameBufferHandle handle);
[[nodiscard]] int getFrameBufferWidth(FrameBufferHandle handle);
[[nodiscard]] int getFrameBufferHeight(FrameBufferHandle handle);

[[nodiscard]] ShaderHandle createShader(std::string_view vertex, std::string_view fragment);
void useShader(ShaderHandle handle);
void destroyShader(ShaderHandle handle);

void setShaderUniform1b(ShaderHandle handle, std::string_view name, bool value);
void setShaderUniform1u(ShaderHandle handle, std::string_view name, unsigned int value);
void setShaderUniform1i(ShaderHandle handle, std::string_view name, int value);
void setShaderUniform1f(ShaderHandle handle, std::string_view name, float value);
void setShaderUniform2b(ShaderHandle handle, std::string_view name, glm::vec2b value);
void setShaderUniform2u(ShaderHandle handle, std::string_view name, glm::vec2u value);
void setShaderUniform2i(ShaderHandle handle, std::string_view name, glm::vec2i value);
void setShaderUniform2f(ShaderHandle handle, std::string_view name, glm::vec2f value);
void setShaderUniform3b(ShaderHandle handle, std::string_view name, glm::vec3b value);
void setShaderUniform3u(ShaderHandle handle, std::string_view name, glm::vec3u value);
void setShaderUniform3i(ShaderHandle handle, std::string_view name, glm::vec3i value);
void setShaderUniform3f(ShaderHandle handle, std::string_view name, glm::vec3f value);
void setShaderUniform4b(ShaderHandle handle, std::string_view name, glm::vec4b value);
void setShaderUniform4u(ShaderHandle handle, std::string_view name, glm::vec4u value);
void setShaderUniform4i(ShaderHandle handle, std::string_view name, glm::vec4i value);
void setShaderUniform4f(ShaderHandle handle, std::string_view name, glm::vec4f value);
void setShaderUniform4m(ShaderHandle handle, std::string_view name, glm::mat4 value);

[[nodiscard]] UniformBufferHandle createUniformBuffer(std::ptrdiff_t size);
void bindUniformBufferToShader(ShaderHandle shaderHandle, UniformBufferHandle uniformBufferHandle, std::string_view name);
void updateUniformBuffer(UniformBufferHandle handle, const void* buffer, std::ptrdiff_t length);
void updateUniformBufferPart(UniformBufferHandle handle, std::ptrdiff_t start, const void* buffer, std::ptrdiff_t length);
void destroyUniformBuffer(UniformBufferHandle handle);

[[nodiscard]] MeshHandle createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode);
void updateMesh(MeshHandle* handle, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode);
void drawMesh(MeshHandle handle, MeshDepthFunction depthFunction, MeshCullType cullType);
void destroyMesh(MeshHandle handle);

void initImGui(SDL_Window* window, SDL_Renderer* renderer);
void startImGuiFrame();
void endImGuiFrame();
void destroyImGui();

} // namespace chira::Renderer
