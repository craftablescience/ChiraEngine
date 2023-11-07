#include "BackendGL.h"

#include <cstddef>
#include <map>
#include <stack>
#include <string>

#include <imgui.h>
#include <ImGuizmo.h>
#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <glad/glversion.h>

#include <core/debug/Assertions.h>
#include <core/debug/Logger.h>

using namespace chira;

CHIRA_CREATE_LOG(GL);

enum class RenderMode {
    CULL_FACE,
    DEPTH_TEST,
    TEXTURE_CUBE_MAP_SEAMLESS,
};

static void changeRenderMode(RenderMode mode, bool enable) {
    constexpr auto changeRenderModeGL = [](int mode, bool enable) -> void {
        if (enable) {
            glEnable(mode);
        } else {
            glDisable(mode);
        }
    };
    switch (mode) {
        case RenderMode::CULL_FACE:
            return changeRenderModeGL(GL_CULL_FACE, enable);
        case RenderMode::DEPTH_TEST:
            return changeRenderModeGL(GL_DEPTH_TEST, enable);
        case RenderMode::TEXTURE_CUBE_MAP_SEAMLESS:
            return changeRenderModeGL(GL_TEXTURE_CUBE_MAP_SEAMLESS, enable);
    }
}

/// State controller to avoid redundant state changes: each state is false by default
std::map<RenderMode, std::stack<bool>> g_GLStates{
        { RenderMode::CULL_FACE, {}, },
        { RenderMode::DEPTH_TEST, {}, },
        { RenderMode::TEXTURE_CUBE_MAP_SEAMLESS, {}, },
};
static void initStates() {
    g_GLStates[RenderMode::CULL_FACE].push(true);
    g_GLStates[RenderMode::DEPTH_TEST].push(true);

    // Wiki says modern hardware is fine with this and it looks better
    g_GLStates[RenderMode::TEXTURE_CUBE_MAP_SEAMLESS].push(true);

    for (const auto& [renderMode, stack] : g_GLStates) {
        changeRenderMode(renderMode, stack.top());
    }
}

static void pushState(RenderMode mode, bool enable) {
    static bool initedStates = false;
    if (!initedStates) {
        initStates();
        initedStates = true;
    }
    chiraAssert(g_GLStates.contains(mode), "This render mode was not added to initStates()!!");
    auto& stack = g_GLStates[mode];
    bool current = stack.top();
    stack.push(enable);
    if (enable != current) {
        changeRenderMode(mode, enable);
    }
}

static void popState(RenderMode mode) {
    if(!g_GLStates.contains(mode) || g_GLStates[mode].size() <= 1) {
        chiraAssert(false, "Attempted to pop render state without a corresponding push!");
    }
    auto& stack = g_GLStates[mode];
    bool old = stack.top();
    stack.pop();
    if (stack.top() != old) {
        changeRenderMode(mode, stack.top());
    }
}

std::string_view Renderer::getHumanName() {
    return GL_VERSION_STRING_PRETTY;
}

bool Renderer::setupForDebugging() {
#if defined(CHIRA_USE_RENDER_BACKEND_GL40) || defined(CHIRA_USE_RENDER_BACKEND_GL41)
    if (!SDL_GL_ExtensionSupported("GL_KHR_debug"))
        return false;
#endif

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT))
        return false;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([](GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei /*length*/, const char* message, const void* /*userParam*/) {
        // Leaving OpenGL error reports unlocalized is probably best

        if (id == 8 || id == 131169 || id == 131185 || id == 131218 || id == 131204) {
            // Ignore 8 because the Steam overlay tries to bind to an already bound framebuffer, very low overhead, don't worry about it
            // Others are ignored because learnopengl.com said they were duplicates
            return;
        }
        std::string output = "Debug message (" + std::to_string(id) + "): " +  message;

        output += "\nSource: ";
        switch (source) {
            case GL_DEBUG_SOURCE_API:             output += "API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   output += "Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: output += "Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     output += "Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     output += "Application"; break;
            default:                              output += "Other";
        }
        output += "\nType: ";
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:               output += "Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: output += "Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  output += "Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         output += "Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         output += "Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              output += "Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          output += "Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           output += "Pop Group"; break;
            default:                                output += "Other";
        }
        output += "\nSeverity: ";
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:         output += "High"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       output += "Medium"; break;
            case GL_DEBUG_SEVERITY_LOW:          output += "Low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: output += "Notification"; break;
            default:                             output += "Other";
        }

        if (type == GL_DEBUG_TYPE_ERROR) {
            LOG_GL.error(output);
        } else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
            LOG_GL.info(output);
        } else {
            // Logging as a warning because most of the time the program runs perfectly fine
            LOG_GL.warning(output);
        }
    }, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    return IMGUI_CHECKVERSION();
}

void Renderer::setClearColor(ColorRGBA color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

[[nodiscard]] static constexpr int getTextureFormatGL(TextureFormat format) {
    switch (format) {
        case TextureFormat::RED:
            return GL_RED;
        case TextureFormat::RG:
            return GL_RG;
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        case TextureFormat::BGR:
            return GL_BGR;
        case TextureFormat::BGRA:
            return GL_BGRA;
        case TextureFormat::RED_INT:
            return GL_RED_INTEGER;
        case TextureFormat::RG_INT:
            return GL_RG_INTEGER;
        case TextureFormat::RGB_INT:
            return GL_RGB_INTEGER;
        case TextureFormat::RGBA_INT:
            return GL_RGBA_INTEGER;
        case TextureFormat::BGR_INT:
            return GL_BGR_INTEGER;
        case TextureFormat::BGRA_INT:
            return GL_BGRA_INTEGER;
        case TextureFormat::STENCIL:
            return GL_STENCIL_INDEX;
        case TextureFormat::DEPTH:
            return GL_DEPTH_COMPONENT;
        case TextureFormat::DEPTH_STENCIL:
            return GL_DEPTH_STENCIL;
    }
    return GL_RGBA;
}

[[nodiscard]] static constexpr int getWrapModeGL(WrapMode mode) {
    switch (mode) {
        case WrapMode::REPEAT:
            return GL_REPEAT;
        case WrapMode::MIRRORED_REPEAT:
            return GL_MIRRORED_REPEAT;
        case WrapMode::CLAMP_TO_EDGE:
            return GL_CLAMP_TO_EDGE;
        case WrapMode::CLAMP_TO_BORDER:
            return GL_CLAMP_TO_BORDER;
    }
    return GL_REPEAT;
}

[[nodiscard]] static constexpr int getFilterModeGL(FilterMode mode) {
    switch (mode) {
        case FilterMode::NEAREST:
            return GL_NEAREST;
        case FilterMode::LINEAR:
            return GL_LINEAR;
    }
    return GL_LINEAR;
}

Renderer::TextureHandle Renderer::createTexture2D(const Image& image, WrapMode wrapS, WrapMode wrapT, FilterMode filter,
                                                  bool genMipmaps, TextureUnit activeTextureUnit) {
    TextureHandle handle{};
    glGenTextures(1, &handle.handle);
    handle.type = TextureType::TWO_DIMENSIONAL;

    const auto glFilter = getFilterModeGL(filter);
    const auto glFormat = getTextureFormatGL(getTextureFormatFromBitDepth(image.getBitDepth()));

    glActiveTexture(GL_TEXTURE0 + static_cast<int>(activeTextureUnit));
    glBindTexture(GL_TEXTURE_2D, handle.handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getWrapModeGL(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getWrapModeGL(wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);

    chiraAssert(image.getData(), "Texture failed to compile: missing image data!");
    if (image.getData()) {
        glTexImage2D(GL_TEXTURE_2D, 0, glFormat, image.getWidth(), image.getHeight(), 0, glFormat, GL_UNSIGNED_BYTE, image.getData());
        if (genMipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    return handle;
}

Renderer::TextureHandle Renderer::createTextureCubemap(const Image& imageRT, const Image& imageLT, const Image& imageUP,
                                                       const Image& imageDN, const Image& imageFD, const Image& imageBK,
                                                       WrapMode wrapS, WrapMode wrapT, WrapMode wrapR, FilterMode filter,
                                                       bool genMipmaps, TextureUnit activeTextureUnit) {
    TextureHandle handle{};
    glGenTextures(1, &handle.handle);
    handle.type = TextureType::CUBEMAP;

    const auto glFilter = getFilterModeGL(filter);

    glActiveTexture(GL_TEXTURE0 + static_cast<int>(activeTextureUnit));
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle.handle);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, getWrapModeGL(wrapS));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, getWrapModeGL(wrapT));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, getWrapModeGL(wrapR));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, glFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, glFilter);

    std::array<const Image*, 6> images{&imageRT, &imageLT, &imageUP, &imageDN, &imageFD, &imageBK};
    for (int i = 0; i < 6; i++) {
        chiraAssert(images[i]->getData(), "Texture failed to compile: missing image data!");
        const auto glFormat = getTextureFormatGL(getTextureFormatFromBitDepth(images[i]->getBitDepth()));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat, images[i]->getWidth(), images[i]->getHeight(), 0, glFormat, GL_UNSIGNED_BYTE, images[i]->getData());
    }
    if (genMipmaps) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
    return handle;
}

void Renderer::useTexture(TextureHandle handle, TextureUnit activeTextureUnit) {
    chiraAssert(static_cast<bool>(handle), "Invalid texture handle given to GL renderer!");
    glActiveTexture(GL_TEXTURE0 + static_cast<int>(activeTextureUnit));
    switch (handle.type) {
        case TextureType::TWO_DIMENSIONAL:
            glBindTexture(GL_TEXTURE_2D, handle.handle);
            break;
        case TextureType::CUBEMAP:
            glBindTexture(GL_TEXTURE_CUBE_MAP, handle.handle);
            break;
    }
}

void* Renderer::getImGuiTextureHandle(Renderer::TextureHandle handle) {
    chiraAssert(handle.type != TextureType::CUBEMAP, "Should probably not be using a cubemap texture in ImGui!");
    return reinterpret_cast<void*>(static_cast<unsigned long long>(handle.handle));
}

void Renderer::destroyTexture(Renderer::TextureHandle handle) {
    chiraAssert(static_cast<bool>(handle), "Invalid texture handle given to GL renderer!");
    glDeleteTextures(1, &handle.handle);
}

std::stack<Renderer::FrameBufferHandle> g_GLFramebuffers{};

Renderer::FrameBufferHandle Renderer::createFrameBuffer(int width, int height, WrapMode wrapS, WrapMode wrapT, FilterMode filter, bool hasDepth) {
    FrameBufferHandle handle{ .hasDepth = hasDepth, .width = width, .height = height, };
    glGenFramebuffers(1, &handle.fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle.fboHandle);

    const auto glFilter = getFilterModeGL(filter);

    glGenTextures(1, &handle.colorHandle);
    glBindTexture(GL_TEXTURE_2D, handle.colorHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getWrapModeGL(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getWrapModeGL(wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle.colorHandle, 0);

    if (hasDepth) {
        glGenRenderbuffers(1, &handle.rboHandle);
        glBindRenderbuffer(GL_RENDERBUFFER, handle.rboHandle);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, handle.rboHandle);
    }

#ifdef DEBUG
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_GL.error("FrameBuffer is not complete! Error: {}", glGetError());
    }
#endif

    glBindFramebuffer(GL_FRAMEBUFFER, g_GLFramebuffers.empty() ? 0 : g_GLFramebuffers.top().fboHandle);
    return handle;
}

void Renderer::pushFrameBuffer(Renderer::FrameBufferHandle handle) {
    auto old = g_GLFramebuffers.empty() ? 0 : g_GLFramebuffers.top().fboHandle;
    g_GLFramebuffers.push(handle);
    if (old != g_GLFramebuffers.top().fboHandle) {
        glViewport(0, 0, g_GLFramebuffers.top().width, g_GLFramebuffers.top().height);
        glBindFramebuffer(GL_FRAMEBUFFER, g_GLFramebuffers.top().fboHandle);
        pushState(RenderMode::DEPTH_TEST, g_GLFramebuffers.top().hasDepth);
    }
    if (g_GLFramebuffers.top().hasDepth) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Renderer::popFrameBuffer() {
    chiraAssert(!g_GLFramebuffers.empty(), "Attempted to pop framebuffer without a corresponding push!");
    auto old = g_GLFramebuffers.top().fboHandle;
    g_GLFramebuffers.pop();
    if (old != (g_GLFramebuffers.empty() ? 0 : g_GLFramebuffers.top().fboHandle)) {
        if (!g_GLFramebuffers.empty()) {
            glViewport(0, 0, g_GLFramebuffers.top().width, g_GLFramebuffers.top().height);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, g_GLFramebuffers.empty() ? 0 : g_GLFramebuffers.top().fboHandle);
        popState(RenderMode::DEPTH_TEST);
    }
}

void Renderer::useFrameBufferTexture(const Renderer::FrameBufferHandle handle, TextureUnit activeTextureUnit) {
    if (handle.colorHandle != 0) {
        glActiveTexture(GL_TEXTURE0 + static_cast<int>(activeTextureUnit));
        glBindTexture(GL_TEXTURE_2D, handle.colorHandle);
    } else if (!handle) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void* Renderer::getImGuiFrameBufferHandle(Renderer::FrameBufferHandle handle) {
    return reinterpret_cast<void*>(static_cast<unsigned long long>(handle.colorHandle));
}

void Renderer::destroyFrameBuffer(Renderer::FrameBufferHandle handle) {
    if (!handle) {
        return;
    }
    if (handle.hasDepth) {
        glDeleteRenderbuffers(1, &handle.rboHandle);
    }
    glDeleteTextures(1, &handle.colorHandle);
    glDeleteFramebuffers(1, &handle.fboHandle);
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
    } else {
        // This is the window framebuffer, just resize it
        handle->width = width;
        handle->height = height;
    }
}

[[nodiscard]] static constexpr int getShaderModuleTypeGL(ShaderModuleType type) {
    switch (type) {
        case ShaderModuleType::VERTEX:
            return GL_VERTEX_SHADER;
        case ShaderModuleType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        CHIRA_NO_DEFAULT;
    }
}

[[nodiscard]] static Renderer::ShaderModuleHandle createShaderModule(std::string_view shader, Renderer::ShaderHandle shaderHandle, ShaderModuleType type) {
    auto data = std::string{GL_VERSION_STRING.data()} + "\n\n" + shader.data();
    const char* dat = data.c_str();

    int glType = getShaderModuleTypeGL(type);
    int handle = glCreateShader(glType);
    glShaderSource(handle, 1, &dat, nullptr);
    glCompileShader(handle);

#ifdef DEBUG
    int success = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512] {0};
        glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);
        LOG_GL.error(fmt::format("Shader module compilation failed: {}", infoLog));
    }
#endif

    glAttachShader(shaderHandle.handle, handle);

    return { .handle = handle };
}

static void destroyShaderModule(Renderer::ShaderModuleHandle handle) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader module handle given to GL renderer!");
    glDeleteShader(handle.handle);
}

Renderer::ShaderHandle Renderer::createShader(std::string_view vertex, std::string_view fragment) {
    ShaderHandle handle{};
    handle.handle = glCreateProgram();
    handle.vertex = createShaderModule(vertex, handle, ShaderModuleType::VERTEX);
    handle.fragment = createShaderModule(fragment, handle, ShaderModuleType::FRAGMENT);
    glLinkProgram(handle.handle);

#ifdef DEBUG
    int success = 0;
    char infoLog[512] {0};
    glGetProgramiv(handle.handle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(handle.handle, sizeof(infoLog), nullptr, infoLog);
        LOG_GL.error(fmt::format("Shader linking failed: {}", infoLog));
    }

    success = 0;
    memset(infoLog, 0, sizeof(infoLog));
    glValidateProgram(handle.handle);
    glGetProgramiv(handle.handle, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(handle.handle, sizeof(infoLog), nullptr, infoLog);
        LOG_GL.error(fmt::format("Shader validation failed: {}", infoLog));
    }
#endif

    return handle;
}

void Renderer::useShader(Renderer::ShaderHandle handle) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUseProgram(handle.handle);
}

void Renderer::destroyShader(Renderer::ShaderHandle handle) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    destroyShaderModule(handle.vertex);
    destroyShaderModule(handle.fragment);
    glDeleteProgram(handle.handle);
}

void Renderer::setShaderUniform1b(Renderer::ShaderHandle handle, std::string_view name, bool value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform1i(glGetUniformLocation(handle.handle, name.data()), static_cast<int>(value));
}

void Renderer::setShaderUniform1u(Renderer::ShaderHandle handle, std::string_view name, unsigned int value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform1ui(glGetUniformLocation(handle.handle, name.data()), value);
}

void Renderer::setShaderUniform1i(Renderer::ShaderHandle handle, std::string_view name, int value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform1i(glGetUniformLocation(handle.handle, name.data()), value);
}

void Renderer::setShaderUniform1f(Renderer::ShaderHandle handle, std::string_view name, float value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform1f(glGetUniformLocation(handle.handle, name.data()), value);
}

void Renderer::setShaderUniform2b(Renderer::ShaderHandle handle, std::string_view name, glm::vec2b value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform2i(glGetUniformLocation(handle.handle, name.data()), static_cast<int>(value.x), static_cast<int>(value.y));
}

void Renderer::setShaderUniform2u(Renderer::ShaderHandle handle, std::string_view name, glm::vec2u value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform2ui(glGetUniformLocation(handle.handle, name.data()), value.x, value.y);
}

void Renderer::setShaderUniform2i(Renderer::ShaderHandle handle, std::string_view name, glm::vec2i value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform2i(glGetUniformLocation(handle.handle, name.data()), value.x, value.y);
}

void Renderer::setShaderUniform2f(Renderer::ShaderHandle handle, std::string_view name, glm::vec2f value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform2f(glGetUniformLocation(handle.handle, name.data()), value.x, value.y);
}

void Renderer::setShaderUniform3b(Renderer::ShaderHandle handle, std::string_view name, glm::vec3b value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform3i(glGetUniformLocation(handle.handle, name.data()), static_cast<int>(value.x), static_cast<int>(value.y), static_cast<int>(value.z));
}

void Renderer::setShaderUniform3u(Renderer::ShaderHandle handle, std::string_view name, glm::vec3u value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform3ui(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z);
}

void Renderer::setShaderUniform3i(Renderer::ShaderHandle handle, std::string_view name, glm::vec3i value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform3i(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z);
}

void Renderer::setShaderUniform3f(Renderer::ShaderHandle handle, std::string_view name, glm::vec3f value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform3f(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z);
}

void Renderer::setShaderUniform4b(Renderer::ShaderHandle handle, std::string_view name, glm::vec4b value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform4i(glGetUniformLocation(handle.handle, name.data()), static_cast<int>(value.x), static_cast<int>(value.y), static_cast<int>(value.z), static_cast<int>(value.w));
}

void Renderer::setShaderUniform4u(Renderer::ShaderHandle handle, std::string_view name, glm::vec4u value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform4ui(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z, value.w);
}

void Renderer::setShaderUniform4i(Renderer::ShaderHandle handle, std::string_view name, glm::vec4i value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform4i(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z, value.w);
}

void Renderer::setShaderUniform4f(Renderer::ShaderHandle handle, std::string_view name, glm::vec4f value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniform4f(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z, value.w);
}

void Renderer::setShaderUniform4m(Renderer::ShaderHandle handle, std::string_view name, glm::mat4 value) {
    chiraAssert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer!");
    glUniformMatrix4fv(glGetUniformLocation(handle.handle, name.data()), 1, GL_FALSE, glm::value_ptr(value));
}

Renderer::UniformBufferHandle Renderer::createUniformBuffer(std::ptrdiff_t size) {
    UniformBufferHandle handle{};

    // The binding point needs to be different for each UBO
    static unsigned int UBO_BINDING_POINT = 0;
    handle.bindingPoint = UBO_BINDING_POINT++;

    glGenBuffers(1, &handle.handle);
    glBindBuffer(GL_UNIFORM_BUFFER, handle.handle);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, handle.bindingPoint);
    glBindBufferRange(GL_UNIFORM_BUFFER, handle.bindingPoint, handle.handle, 0, size);

    return handle;
}

void Renderer::bindUniformBufferToShader(Renderer::ShaderHandle shaderHandle, Renderer::UniformBufferHandle uniformBufferHandle, std::string_view name) {
    chiraAssert(static_cast<bool>(shaderHandle), "Invalid shader handle given to GL renderer!");
    chiraAssert(static_cast<bool>(uniformBufferHandle), "Invalid uniform buffer handle given to GL renderer!");
    glUniformBlockBinding(shaderHandle.handle, glGetUniformBlockIndex(shaderHandle.handle, name.data()), uniformBufferHandle.bindingPoint);
}

void Renderer::updateUniformBuffer(Renderer::UniformBufferHandle handle, const void* buffer, std::ptrdiff_t length) {
    chiraAssert(static_cast<bool>(handle), "Invalid uniform buffer handle given to GL renderer!");
    glBindBuffer(GL_UNIFORM_BUFFER, handle.handle);
    glBufferData(GL_UNIFORM_BUFFER, length, buffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateUniformBufferPart(Renderer::UniformBufferHandle handle, std::ptrdiff_t start, const void* buffer, std::ptrdiff_t length) {
    chiraAssert(static_cast<bool>(handle), "Invalid uniform buffer handle given to GL renderer!");
    glBindBuffer(GL_UNIFORM_BUFFER, handle.handle);
    glBufferSubData(GL_UNIFORM_BUFFER, start, length, buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::destroyUniformBuffer(Renderer::UniformBufferHandle handle) {
    chiraAssert(static_cast<bool>(handle), "Invalid uniform buffer handle given to GL renderer!");
    glDeleteBuffers(1, &handle.handle);
}

[[nodiscard]] static constexpr int getMeshDrawModeGL(MeshDrawMode mode) {
    switch (mode) {
        case MeshDrawMode::STATIC:
            return GL_STATIC_DRAW;
        case MeshDrawMode::DYNAMIC:
            return GL_DYNAMIC_DRAW;
    }
    return GL_STATIC_DRAW;
}

[[nodiscard]] static constexpr int getMeshDepthFunctionGL(MeshDepthFunction function) {
    switch (function) {
        case MeshDepthFunction::NEVER:
            return GL_NEVER;
        case MeshDepthFunction::ALWAYS:
            return GL_ALWAYS;
        case MeshDepthFunction::EQUAL:
            return GL_EQUAL;
        case MeshDepthFunction::NOTEQUAL:
            return GL_NOTEQUAL;
        case MeshDepthFunction::LESS:
            return GL_LESS;
        case MeshDepthFunction::LEQUAL:
            return GL_LEQUAL;
        case MeshDepthFunction::GREATER:
            return GL_GREATER;
        case MeshDepthFunction::GEQUAL:
            return GL_GEQUAL;
    }
    return GL_LEQUAL;
}

[[nodiscard]] static constexpr int getMeshCullTypeGL(MeshCullType type) {
    switch (type) {
        case MeshCullType::BACK:
            return GL_BACK;
        case MeshCullType::FRONT:
            return GL_FRONT;
        case MeshCullType::NONE:
            return GL_FRONT_AND_BACK;
    }
    return GL_BACK;
}

Renderer::MeshHandle Renderer::createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode) {
    MeshHandle handle{ .numIndices = static_cast<int>(indices.size()) };
    glGenVertexArrays(1, &handle.vaoHandle);
    glGenBuffers(1, &handle.vboHandle);
    glGenBuffers(1, &handle.eboHandle);

    glBindVertexArray(handle.vaoHandle);

    const auto glDrawMode = getMeshDrawModeGL(drawMode);

    glBindBuffer(GL_ARRAY_BUFFER, handle.vboHandle);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), glDrawMode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(Index)), indices.data(), glDrawMode);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(2);
    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return handle;
}

void Renderer::updateMesh(MeshHandle* handle, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode) {
    chiraAssert(static_cast<bool>(*handle), "Invalid mesh handle given to GL renderer!");
    const auto glDrawMode = getMeshDrawModeGL(drawMode);
    glBindBuffer(GL_ARRAY_BUFFER, handle->vboHandle);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), glDrawMode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle->eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(Index)), indices.data(), glDrawMode);
    handle->numIndices = static_cast<int>(indices.size());
}

void Renderer::drawMesh(MeshHandle handle, MeshDepthFunction depthFunction, MeshCullType cullType) {
    chiraAssert(static_cast<bool>(handle), "Invalid mesh handle given to GL renderer!");
    pushState(RenderMode::CULL_FACE, true);
    glDepthFunc(getMeshDepthFunctionGL(depthFunction));
    glCullFace(getMeshCullTypeGL(cullType));
    glBindVertexArray(handle.vaoHandle);
    glDrawElements(GL_TRIANGLES, handle.numIndices, GL_UNSIGNED_INT, nullptr);
    popState(RenderMode::CULL_FACE);
}

void Renderer::destroyMesh(MeshHandle handle) {
    chiraAssert(static_cast<bool>(handle), "Invalid mesh handle given to GL renderer!");
    glDeleteVertexArrays(1, &handle.vaoHandle);
    glDeleteBuffers(1, &handle.vboHandle);
    glDeleteBuffers(1, &handle.eboHandle);
}

void Renderer::initImGui(SDL_Window* window, void* context) {
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init(GL_VERSION_STRING.data());
}

void Renderer::startImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_PassthruCentralNode);
}

void Renderer::endImGuiFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::destroyImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
}
