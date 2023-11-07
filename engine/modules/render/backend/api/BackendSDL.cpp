#include "BackendSDL.h"

#include <cstddef>
#include <map>
#include <stack>
#include <string>

#include <imgui.h>
#include <ImGuizmo.h>
#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer.h>
#include <glm/gtc/type_ptr.hpp>

#include <core/debug/Assertions.h>
#include <core/debug/Logger.h>

#define STUBFUNC(name) LOG_SDLRENDER.error(#name " is not currently implemented!");
#define UNSUPPORTED(name) LOG_SDLRENDER.warning(#name " is not supported under SDL Renderer!");

using namespace chira;

CHIRA_CREATE_LOG(SDLRENDER);

SDL_Renderer* g_Renderer = nullptr;

enum class RenderMode {
    CULL_FACE,
    DEPTH_TEST,
    TEXTURE_CUBE_MAP_SEAMLESS,
};

static void changeRenderMode(RenderMode mode, bool enable) {
    STUBFUNC(changeRenderMode);
}

/// State controller to avoid redundant state changes: each state is false by default
std::map<RenderMode, std::stack<bool>> g_SDLStates{
        { RenderMode::CULL_FACE, {}, },
        { RenderMode::DEPTH_TEST, {}, },
        { RenderMode::TEXTURE_CUBE_MAP_SEAMLESS, {}, },
};
static void initStates() {
    g_SDLStates[RenderMode::CULL_FACE].push(true);
    g_SDLStates[RenderMode::DEPTH_TEST].push(true);

    // Wiki says modern hardware is fine with this and it looks better
    g_SDLStates[RenderMode::TEXTURE_CUBE_MAP_SEAMLESS].push(true);

    for (const auto& [renderMode, stack] : g_SDLStates) {
        changeRenderMode(renderMode, stack.top());
    }
}

static void pushState(RenderMode mode, bool enable) {
    static bool initedStates = false;
    if (!initedStates) {
        initStates();
        initedStates = true;
    }
    chiraAssert(g_SDLStates.contains(mode), "This render mode was not added to initStates()!!");
    auto& stack = g_SDLStates[mode];
    bool current = stack.top();
    stack.push(enable);
    if (enable != current) {
        changeRenderMode(mode, enable);
    }
}

static void popState(RenderMode mode) {
    if (!g_SDLStates.contains(mode) || g_SDLStates[mode].size() <= 1) {
        chiraAssert(false, "Attempted to pop render state without a corresponding push!");
    }
    auto& stack = g_SDLStates[mode];
    bool old = stack.top();
    stack.pop();
    if (stack.top() != old) {
        changeRenderMode(mode, stack.top());
    }
}

std::string_view Renderer::getHumanName() {
    return "SDL Renderer";
}

bool Renderer::setupForDebugging() {
    STUBFUNC(setupForDebugging);
    return IMGUI_CHECKVERSION();
}

void Renderer::setClearColor(ColorRGBA color) {
    float r = color.r * 255;
    float g = color.g * 255;
    float b = color.b * 255;
    float a = color.a * 255;

    SDL_SetRenderDrawColor(g_Renderer,
        static_cast<int>(r),
        static_cast<int>(g),
        static_cast<int>(b),
        static_cast<int>(a)
    );
}

//[[nodiscard]] static constexpr int getTextureFormatGL(TextureFormat format) {
//    switch (format) {
//    case TextureFormat::RED:
//        return GL_RED;
//    case TextureFormat::RG:
//        return GL_RG;
//    case TextureFormat::RGB:
//        return GL_RGB;
//    case TextureFormat::RGBA:
//        return GL_RGBA;
//    case TextureFormat::BGR:
//        return GL_BGR;
//    case TextureFormat::BGRA:
//        return GL_BGRA;
//    case TextureFormat::RED_INT:
//        return GL_RED_INTEGER;
//    case TextureFormat::RG_INT:
//        return GL_RG_INTEGER;
//    case TextureFormat::RGB_INT:
//        return GL_RGB_INTEGER;
//    case TextureFormat::RGBA_INT:
//        return GL_RGBA_INTEGER;
//    case TextureFormat::BGR_INT:
//        return GL_BGR_INTEGER;
//    case TextureFormat::BGRA_INT:
//        return GL_BGRA_INTEGER;
//    case TextureFormat::STENCIL:
//        return GL_STENCIL_INDEX;
//    case TextureFormat::DEPTH:
//        return GL_DEPTH_COMPONENT;
//    case TextureFormat::DEPTH_STENCIL:
//        return GL_DEPTH_STENCIL;
//    }
//    return GL_RGBA;
//}
//
//[[nodiscard]] static constexpr int getWrapModeGL(WrapMode mode) {
//    switch (mode) {
//    case WrapMode::REPEAT:
//        return GL_REPEAT;
//    case WrapMode::MIRRORED_REPEAT:
//        return GL_MIRRORED_REPEAT;
//    case WrapMode::CLAMP_TO_EDGE:
//        return GL_CLAMP_TO_EDGE;
//    case WrapMode::CLAMP_TO_BORDER:
//        return GL_CLAMP_TO_BORDER;
//    }
//    return GL_REPEAT;
//}
//
//[[nodiscard]] static constexpr int getFilterModeGL(FilterMode mode) {
//    switch (mode) {
//    case FilterMode::NEAREST:
//        return GL_NEAREST;
//    case FilterMode::LINEAR:
//        return GL_LINEAR;
//    }
//    return GL_LINEAR;
//}

Renderer::TextureHandle Renderer::createTexture2D(const Image& image, WrapMode wrapS, WrapMode wrapT, FilterMode filter,
    bool genMipmaps, TextureUnit activeTextureUnit) {
    TextureHandle handle{};
    handle.type = TextureType::TWO_DIMENSIONAL;

    STUBFUNC(createTexture2D);

    //chiraAssert(image.getData(), "Texture failed to compile: missing image data!");
    return handle;
}

Renderer::TextureHandle Renderer::createTextureCubemap(const Image& imageRT, const Image& imageLT, const Image& imageUP,
    const Image& imageDN, const Image& imageFD, const Image& imageBK,
    WrapMode wrapS, WrapMode wrapT, WrapMode wrapR, FilterMode filter,
    bool genMipmaps, TextureUnit activeTextureUnit) {
    TextureHandle handle{};
    handle.type = TextureType::CUBEMAP;

    STUBFUNC(createTextureCubemap);

    return handle;
}

void Renderer::useTexture(TextureHandle handle, TextureUnit activeTextureUnit) {
    //chiraAssert(static_cast<bool>(handle), "Invalid texture handle given to SDL renderer!");
    //STUBFUNC(useTexture);
    switch (handle.type) {
    case TextureType::TWO_DIMENSIONAL:
        break;
    case TextureType::CUBEMAP:
        break;
    }
}

void* Renderer::getImGuiTextureHandle(Renderer::TextureHandle handle) {
    //chiraAssert(handle.type != TextureType::CUBEMAP, "Should probably not be using a cubemap texture in ImGui!");
    return reinterpret_cast<void*>(static_cast<unsigned long long>(handle.handle));
}

void Renderer::destroyTexture(Renderer::TextureHandle handle) {
    //chiraAssert(static_cast<bool>(handle), "Invalid texture handle given to SDL renderer!");
    STUBFUNC(destroyTexture);
}

std::stack<Renderer::FrameBufferHandle> g_SDLFramebuffers{};

Renderer::FrameBufferHandle Renderer::createFrameBuffer(int width, int height, WrapMode wrapS, WrapMode wrapT, FilterMode filter, bool hasDepth) {
    FrameBufferHandle handle{ .hasDepth = hasDepth, .width = width, .height = height, };
    handle.texture = SDL_CreateTexture(g_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
    return handle;
}

void Renderer::pushFrameBuffer(Renderer::FrameBufferHandle handle) {
    auto old = g_SDLFramebuffers.empty() ? 0 : g_SDLFramebuffers.top().texture;
    g_SDLFramebuffers.push(handle);
    if (old != g_SDLFramebuffers.top().texture) {
        SDL_Rect viewport;
        viewport.x = 0;
        viewport.y = 0;
        viewport.w = g_SDLFramebuffers.top().width;
        viewport.h = g_SDLFramebuffers.top().height;
        SDL_RenderSetViewport(g_Renderer, &viewport);
    }
    SDL_SetRenderTarget(g_Renderer, g_SDLFramebuffers.top().texture);
    SDL_RenderClear(g_Renderer);
}

void Renderer::popFrameBuffer() {
    chiraAssert(!g_SDLFramebuffers.empty(), "Attempted to pop framebuffer without a corresponding push!");
    auto old = g_SDLFramebuffers.top().texture;
    g_SDLFramebuffers.pop();
    if (old != (g_SDLFramebuffers.empty() ? 0 : g_SDLFramebuffers.top().texture)) {
        if (!g_SDLFramebuffers.empty()) {
            SDL_Rect viewport;
            viewport.x = 0;
            viewport.y = 0;
            viewport.w = g_SDLFramebuffers.top().width;
            viewport.h = g_SDLFramebuffers.top().height;
            SDL_RenderSetViewport(g_Renderer, &viewport);
        }
        SDL_SetRenderTarget(g_Renderer, g_SDLFramebuffers.empty() ? nullptr : g_SDLFramebuffers.top().texture);
        // copy texture over
        SDL_RenderCopy(g_Renderer, old, nullptr, nullptr);
    }
}

void Renderer::useFrameBufferTexture(const Renderer::FrameBufferHandle handle, TextureUnit activeTextureUnit) {
    STUBFUNC(useFrameBufferTexture);
}

void* Renderer::getImGuiFrameBufferHandle(Renderer::FrameBufferHandle handle) {
    return reinterpret_cast<void*>(static_cast<unsigned long long>(handle.colorHandle));
}

void Renderer::destroyFrameBuffer(Renderer::FrameBufferHandle handle) {
    if (!handle) {
        return;
    }
    SDL_DestroyTexture(handle.texture);
}

int Renderer::getFrameBufferWidth(Renderer::FrameBufferHandle handle) {
    return handle.width;
}

int Renderer::getFrameBufferHeight(Renderer::FrameBufferHandle handle) {
    return handle.height;
}

void Renderer::recreateFrameBuffer(Renderer::FrameBufferHandle* handle, int width, int height, WrapMode wrapS, WrapMode wrapT, FilterMode filter, bool hasDepth) {
    if (!handle)
        return;
    if (*handle) {
        destroyFrameBuffer(*handle);
        *handle = Renderer::createFrameBuffer(width, height, wrapS, wrapT, filter, hasDepth);
    }
    else {
        // This is the window framebuffer, just resize it
        handle->width = width;
        handle->height = height;
    }
}


[[nodiscard]] static Renderer::ShaderModuleHandle createShaderModule(std::string_view shader, Renderer::ShaderHandle shaderHandle, ShaderModuleType type) {
    UNSUPPORTED(createShaderModule);
    return { };
}

static void destroyShaderModule(Renderer::ShaderModuleHandle handle) {
    UNSUPPORTED(destroyShaderModule);
}

Renderer::ShaderHandle Renderer::createShader(std::string_view vertex, std::string_view fragment) {
    ShaderHandle handle{};
    UNSUPPORTED(createShader);
    return handle;
}

void Renderer::useShader(Renderer::ShaderHandle handle) {
    // don't print unsupported on this as it spams the console.
    //UNSUPPORTED(useShader);
}

void Renderer::destroyShader(Renderer::ShaderHandle handle) {
    UNSUPPORTED(destroyShader);
}

void Renderer::setShaderUniform1b(Renderer::ShaderHandle handle, std::string_view name, bool value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform1b);
}

void Renderer::setShaderUniform1u(Renderer::ShaderHandle handle, std::string_view name, unsigned int value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform1u);
}

void Renderer::setShaderUniform1i(Renderer::ShaderHandle handle, std::string_view name, int value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform1i);
}

void Renderer::setShaderUniform1f(Renderer::ShaderHandle handle, std::string_view name, float value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform1f);
}

void Renderer::setShaderUniform2b(Renderer::ShaderHandle handle, std::string_view name, glm::vec2b value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform2b);
}

void Renderer::setShaderUniform2u(Renderer::ShaderHandle handle, std::string_view name, glm::vec2u value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform2u);
}

void Renderer::setShaderUniform2i(Renderer::ShaderHandle handle, std::string_view name, glm::vec2i value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform2i);
}

void Renderer::setShaderUniform2f(Renderer::ShaderHandle handle, std::string_view name, glm::vec2f value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform2f);
}

void Renderer::setShaderUniform3b(Renderer::ShaderHandle handle, std::string_view name, glm::vec3b value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform3b);
}

void Renderer::setShaderUniform3u(Renderer::ShaderHandle handle, std::string_view name, glm::vec3u value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform3u);
}

void Renderer::setShaderUniform3i(Renderer::ShaderHandle handle, std::string_view name, glm::vec3i value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform3i);
}

void Renderer::setShaderUniform3f(Renderer::ShaderHandle handle, std::string_view name, glm::vec3f value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform3f);
}

void Renderer::setShaderUniform4b(Renderer::ShaderHandle handle, std::string_view name, glm::vec4b value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform4b);
}

void Renderer::setShaderUniform4u(Renderer::ShaderHandle handle, std::string_view name, glm::vec4u value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform4u);
}

void Renderer::setShaderUniform4i(Renderer::ShaderHandle handle, std::string_view name, glm::vec4i value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform4i);
}

void Renderer::setShaderUniform4f(Renderer::ShaderHandle handle, std::string_view name, glm::vec4f value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform4f);
}

void Renderer::setShaderUniform4m(Renderer::ShaderHandle handle, std::string_view name, glm::mat4 value) {
    // Don't print unsupported on this as it spams the console
    //UNSUPPORTED(setShaderUniform4m);
}

Renderer::UniformBufferHandle Renderer::createUniformBuffer(std::ptrdiff_t size) {
    UniformBufferHandle handle{};

    STUBFUNC(createUniformBuffer);

    return handle;
}

void Renderer::bindUniformBufferToShader(Renderer::ShaderHandle shaderHandle, Renderer::UniformBufferHandle uniformBufferHandle, std::string_view name) {
    UNSUPPORTED(bindUniformBufferToShader);
}

void Renderer::updateUniformBuffer(Renderer::UniformBufferHandle handle, const void* buffer, std::ptrdiff_t length) {
    //STUBFUNC(updateUniformBuffer);
}

void Renderer::updateUniformBufferPart(Renderer::UniformBufferHandle handle, std::ptrdiff_t start, const void* buffer, std::ptrdiff_t length) {
    //STUBFUNC(updateUniformBufferPart);
}

void Renderer::destroyUniformBuffer(Renderer::UniformBufferHandle handle) {
    STUBFUNC(destroyUniformBuffer);
}

Renderer::MeshHandle Renderer::createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode) {
    MeshHandle handle{ 
        .numIndices = static_cast<int>(indices.size()),
        .numVertices = static_cast<int>(vertices.size())
    };

    for (Index i : indices) {
        handle.indices.push_back(static_cast<int>(i));
    }

    handle.vertices = vertices;

    return handle;
}

void Renderer::updateMesh(MeshHandle* handle, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode) {
    chiraAssert(static_cast<bool>(*handle), "Invalid mesh handle given to SDL renderer!");
    handle->indices.clear();
    for (Index i : indices) {
        handle->indices.push_back(static_cast<int>(i));
    }

    handle->vertices.clear();
    handle->vertices = vertices;

    handle->numIndices = static_cast<int>(indices.size());
    handle->numVertices = static_cast<int>(vertices.size());
}

void Renderer::drawMesh(MeshHandle handle, MeshDepthFunction depthFunction, MeshCullType cullType) {
    chiraAssert(static_cast<bool>(handle), "Invalid mesh handle given to SDL renderer!");
    std::vector<SDL_Vertex> vertices;

    for (Vertex v : handle.vertices) {
        SDL_Vertex nv;

        nv.position.x = (v.position.x / v.position.z) * 0.5;
        nv.position.y = (v.position.y / v.position.z) * 0.5;

        nv.color.r = 0;
        nv.color.g = 155;
        nv.color.b = 255;

        nv.tex_coord.x = v.uv.r;
        nv.tex_coord.y = v.uv.g;

        vertices.push_back(nv);
    }

    SDL_RenderGeometry(g_Renderer, nullptr, 
        vertices.data(), handle.numVertices,
        handle.indices.data(), handle.numIndices
    );
}

void Renderer::destroyMesh(MeshHandle handle) {
    chiraAssert(static_cast<bool>(handle), "Invalid mesh handle given to SDL renderer!");
    STUBFUNC(destroyMesh);
}

void Renderer::initImGui(SDL_Window* window, SDL_Renderer* renderer) {
    g_Renderer = renderer;
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
}

void Renderer::startImGuiFrame() {
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_PassthruCentralNode);
}

void Renderer::endImGuiFrame() {
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::destroyImGui() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
}
