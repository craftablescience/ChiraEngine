#include "RenderBackendGL.h"

#include <cstddef>
#include <cstring>
#include <map>
#include <stack>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <glad/glversion.h>
#include <backends/imgui_impl_opengl3.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <core/Assertions.h>
#include <core/Logger.h>
#include <i18n/TranslationManager.h>

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
static std::map<RenderMode, std::stack<bool>> GL_STATES{
        { RenderMode::CULL_FACE, {}, },
        { RenderMode::DEPTH_TEST, {}, },
        { RenderMode::TEXTURE_CUBE_MAP_SEAMLESS, {}, },
};
static void initStates() {
    GL_STATES[RenderMode::CULL_FACE].push(true);
    GL_STATES[RenderMode::DEPTH_TEST].push(true);

    // Wiki says modern hardware is fine with this and it looks better
    GL_STATES[RenderMode::TEXTURE_CUBE_MAP_SEAMLESS].push(true);

    for (const auto& [renderMode, stack] : GL_STATES) {
        changeRenderMode(renderMode, stack.top());
    }
}

static void pushState(RenderMode mode, bool enable) {
    static bool initedStates = false;
    if (!initedStates) {
        initStates();
        initedStates = true;
    }
    runtime_assert(GL_STATES.contains(mode), "This render mode was not added to initStates()!!");
    auto& stack = GL_STATES[mode];
    bool current = stack.top();
    stack.push(enable);
    if (enable != current) {
        changeRenderMode(mode, enable);
    }
}

static void popState(RenderMode mode) {
    if(!GL_STATES.contains(mode) || GL_STATES[mode].size() <= 1) {
        runtime_assert(false, "Attempted to pop render state without a corresponding push!");
    }
    auto& stack = GL_STATES[mode];
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
#ifdef CHIRA_USE_GL_41
    if (!glfwExtensionSupported("GL_KHR_debug"))
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
                                                  bool genMipmaps /*= true*/, TextureUnit activeTextureUnit /*= TextureUnit::G0*/) {
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

    runtime_assert(image.getData(), "Texture failed to compile: missing image data");
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
                                                       bool genMipmaps /*= true*/, TextureUnit activeTextureUnit /*= TextureUnit::G0*/) {
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
        runtime_assert(images[i]->getData(), "Texture failed to compile: missing image data");
        const auto glFormat = getTextureFormatGL(getTextureFormatFromBitDepth(images[i]->getBitDepth()));
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat, images[i]->getWidth(), images[i]->getHeight(), 0, glFormat, GL_UNSIGNED_BYTE, images[i]->getData());
    }
    if (genMipmaps) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
    return handle;
}

void Renderer::useTexture(TextureHandle handle, TextureUnit activeTextureUnit /*= TextureUnit::G0*/) {
    runtime_assert(static_cast<bool>(handle), "Invalid texture handle given to GL renderer");
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

void Renderer::destroyTexture(Renderer::TextureHandle handle) {
    runtime_assert(static_cast<bool>(handle), "Invalid texture handle given to GL renderer");
    glDeleteTextures(1, &handle.handle);
}

static std::stack<Renderer::FrameBufferHandle> GL_FRAMEBUFFERS{};

Renderer::FrameBufferHandle Renderer::createFrameBuffer(int width, int height, WrapMode wrapS, WrapMode wrapT, FilterMode filter, bool hasDepth /*= true*/) {
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
        LOG_GL.error("FrameBuffer is not complete!");
    }
#endif

    glBindFramebuffer(GL_FRAMEBUFFER, GL_FRAMEBUFFERS.empty() ? 0 : GL_FRAMEBUFFERS.top().fboHandle);
    return handle;
}

void Renderer::pushFrameBuffer(Renderer::FrameBufferHandle handle) {
    auto old = GL_FRAMEBUFFERS.empty() ? 0 : GL_FRAMEBUFFERS.top().fboHandle;
    GL_FRAMEBUFFERS.push(handle);
    if (old != GL_FRAMEBUFFERS.top().fboHandle) {
        glViewport(0, 0, GL_FRAMEBUFFERS.top().width, GL_FRAMEBUFFERS.top().height);
        glBindFramebuffer(GL_FRAMEBUFFER, GL_FRAMEBUFFERS.top().fboHandle);
        pushState(RenderMode::DEPTH_TEST, GL_FRAMEBUFFERS.top().hasDepth);
        if (GL_FRAMEBUFFERS.top().hasDepth) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        } else {
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }
}

void Renderer::popFrameBuffer() {
    runtime_assert(!GL_FRAMEBUFFERS.empty(), "Attempted to pop framebuffer without a corresponding push!");
    auto old = GL_FRAMEBUFFERS.top().fboHandle;
    GL_FRAMEBUFFERS.pop();
    if (old != (GL_FRAMEBUFFERS.empty() ? 0 : GL_FRAMEBUFFERS.top().fboHandle)) {
        if (!GL_FRAMEBUFFERS.empty()) {
            // todo(render): this looks ugly, fix this when windowing is ported
            glViewport(0, 0, GL_FRAMEBUFFERS.top().width, GL_FRAMEBUFFERS.top().height);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, GL_FRAMEBUFFERS.empty() ? 0 : GL_FRAMEBUFFERS.top().fboHandle);
        popState(RenderMode::DEPTH_TEST);
    }
}

void Renderer::destroyFrameBuffer(Renderer::FrameBufferHandle handle) {
    runtime_assert(static_cast<bool>(handle), "Invalid framebuffer handle given to GL renderer");
    if (handle.hasDepth) {
        glDeleteRenderbuffers(1, &handle.rboHandle);
    }
    glDeleteTextures(1, &handle.colorHandle);
    glDeleteFramebuffers(1, &handle.fboHandle);
}

[[nodiscard]] static constexpr int getShaderModuleTypeGL(ShaderModuleType type) {
    switch (type) {
        case ShaderModuleType::VERTEX:
            return GL_VERTEX_SHADER;
        case ShaderModuleType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
    }
    return -1;
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
    char infoLog[512] {0};
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);
        LOG_GL.error(fmt::format("Shader module compilation failed: {}", infoLog));
    }
#endif

    glAttachShader(shaderHandle.handle, handle);

    return { .handle = handle };
}

static void destroyShaderModule(Renderer::ShaderModuleHandle handle) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader module handle given to GL renderer");
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
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUseProgram(handle.handle);
}

void Renderer::destroyShader(Renderer::ShaderHandle handle) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    destroyShaderModule(handle.vertex);
    destroyShaderModule(handle.fragment);
    glDeleteProgram(handle.handle);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, bool value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform1i(glGetUniformLocation(handle.handle, name.data()), static_cast<int>(value));
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, int value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform1i(glGetUniformLocation(handle.handle, name.data()), value);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, unsigned int value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform1ui(glGetUniformLocation(handle.handle, name.data()), value);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, float value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform1f(glGetUniformLocation(handle.handle, name.data()), value);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec2b value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform2i(glGetUniformLocation(handle.handle, name.data()), static_cast<int>(value.x), static_cast<int>(value.y));
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec2ui value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform2ui(glGetUniformLocation(handle.handle, name.data()), value.x, value.y);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec2i value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform2i(glGetUniformLocation(handle.handle, name.data()), value.x, value.y);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec2f value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform2f(glGetUniformLocation(handle.handle, name.data()), value.x, value.y);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec3b value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform3i(glGetUniformLocation(handle.handle, name.data()), static_cast<int>(value.x), static_cast<int>(value.y), static_cast<int>(value.z));
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec3ui value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform3ui(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec3i value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform3i(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec3f value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform3f(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec4b value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform4i(glGetUniformLocation(handle.handle, name.data()), static_cast<int>(value.x), static_cast<int>(value.y), static_cast<int>(value.z), static_cast<int>(value.w));
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec4ui value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform4ui(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z, value.w);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec4i value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform4i(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z, value.w);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::vec4f value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
    glUniform4f(glGetUniformLocation(handle.handle, name.data()), value.x, value.y, value.z, value.w);
}

void Renderer::setShaderUniform(Renderer::ShaderHandle handle, std::string_view name, glm::mat4 value) {
    runtime_assert(static_cast<bool>(handle), "Invalid shader handle given to GL renderer");
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
    runtime_assert(static_cast<bool>(shaderHandle), "Invalid shader handle given to GL renderer");
    runtime_assert(static_cast<bool>(uniformBufferHandle), "Invalid uniform buffer handle given to GL renderer");
    glUniformBlockBinding(shaderHandle.handle, glGetUniformBlockIndex(shaderHandle.handle, name.data()), uniformBufferHandle.bindingPoint);
}

void Renderer::updateUniformBuffer(Renderer::UniformBufferHandle handle, const void* buffer, std::ptrdiff_t length) {
    runtime_assert(static_cast<bool>(handle), "Invalid uniform buffer handle given to GL renderer");
    glBindBuffer(GL_UNIFORM_BUFFER, handle.handle);
    glBufferData(GL_UNIFORM_BUFFER, length, buffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::updateUniformBufferPart(Renderer::UniformBufferHandle handle, std::ptrdiff_t start, const void* buffer, std::ptrdiff_t length) {
    runtime_assert(static_cast<bool>(handle), "Invalid uniform buffer handle given to GL renderer");
    glBindBuffer(GL_UNIFORM_BUFFER, handle.handle);
    glBufferSubData(GL_UNIFORM_BUFFER, start, length, buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::destroyUniformBuffer(Renderer::UniformBufferHandle handle) {
    runtime_assert(static_cast<bool>(handle), "Invalid uniform buffer handle given to GL renderer");
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
    MeshHandle handle{};
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

void Renderer::updateMesh(MeshHandle handle, const std::vector<Vertex>& vertices, const std::vector<Index>& indices, MeshDrawMode drawMode) {
    runtime_assert(static_cast<bool>(handle), "Invalid mesh handle given to GL renderer");
    const auto glDrawMode = getMeshDrawModeGL(drawMode);
    glBindBuffer(GL_ARRAY_BUFFER, handle.vboHandle);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), glDrawMode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle.eboHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(Index)), indices.data(), glDrawMode);
}

void Renderer::drawMesh(MeshHandle handle, const std::vector<Index>& indices, MeshDepthFunction depthFunction, MeshCullType cullType) {
    runtime_assert(static_cast<bool>(handle), "Invalid mesh handle given to GL renderer");
    pushState(RenderMode::CULL_FACE, true);
    glDepthFunc(getMeshDepthFunctionGL(depthFunction));
    glCullFace(getMeshCullTypeGL(cullType));
    glBindVertexArray(handle.vaoHandle);
    glDrawElements(GL_TRIANGLES, static_cast<GLint>(indices.size()), GL_UNSIGNED_INT, nullptr);
    popState(RenderMode::CULL_FACE);
}

void Renderer::destroyMesh(MeshHandle handle) {
    runtime_assert(static_cast<bool>(handle), "Invalid mesh handle given to GL renderer");
    glDeleteVertexArrays(1, &handle.vaoHandle);
    glDeleteBuffers(1, &handle.vboHandle);
    glDeleteBuffers(1, &handle.eboHandle);
}
