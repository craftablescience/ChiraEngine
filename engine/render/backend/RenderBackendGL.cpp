#include "RenderBackendGL.h"

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

std::string_view RenderBackendGL::getHumanName() {
    return GL_VERSION_STRING_PRETTY;
}

bool RenderBackendGL::setupForDebugging() {
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
        std::string output = "---------------\nDebug message (" + std::to_string(id) + "): " +  message;

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

int RenderBackendGL::getTextureFormat(TextureFormat format) {
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

int RenderBackendGL::getTextureFormatFromBitDepth(int bd) {
    switch (bd) {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        default:
        case 4:
            return GL_RGBA;
    }
}

int RenderBackendGL::getWrapMode(WrapMode mode) {
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

int RenderBackendGL::getFilterMode(FilterMode mode) {
    switch (mode) {
        case FilterMode::NEAREST:
            return GL_NEAREST;
        case FilterMode::LINEAR:
            return GL_LINEAR;
    }
    return GL_LINEAR;
}

unsigned int RenderBackendGL::createTexture2D(const Image& image, WrapMode wrapS, WrapMode wrapT, FilterMode filter,
                                              bool genMipmaps /*= true*/, TextureUnit activeTextureUnit /*= TextureUnit::G0*/) {
    unsigned int handle;
    glGenTextures(1, &handle);

    const auto glFilter = getFilterMode(filter);
    const auto glFormat = getTextureFormatFromBitDepth(image.getBitDepth());

    glActiveTexture(GL_TEXTURE0 + static_cast<int>(activeTextureUnit));
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getWrapMode(wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getWrapMode(wrapT));
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

unsigned int RenderBackendGL::createTextureCubemap(const Image& imageRT, const Image& imageLT, const Image& imageUP,
                                                   const Image& imageDN, const Image& imageFD, const Image& imageBK,
                                                   WrapMode wrapS, WrapMode wrapT, WrapMode wrapR, FilterMode filter,
                                                   bool genMipmaps /*= true*/, TextureUnit activeTextureUnit /*= TextureUnit::G0*/) {
    unsigned int handle;
    glGenTextures(1, &handle);

    const auto glFilter = getFilterMode(filter);

    glActiveTexture(GL_TEXTURE0 + static_cast<int>(activeTextureUnit));
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, getWrapMode(wrapS));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, getWrapMode(wrapT));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, getWrapMode(wrapR));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, glFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, glFilter);

    std::array<const Image*, 6> images{&imageRT, &imageLT, &imageUP, &imageDN, &imageFD, &imageBK};
    for (int i = 0; i < 6; i++) {
        runtime_assert(images[i]->getData(), "Texture failed to compile: missing image data");
        const auto glFormat = getTextureFormatFromBitDepth(images[i]->getBitDepth());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat, images[i]->getWidth(), images[i]->getHeight(), 0, glFormat, GL_UNSIGNED_BYTE, images[i]->getData());
    }
    if (genMipmaps) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
    return handle;
}

void RenderBackendGL::useTexture(TextureType type, unsigned int handle, TextureUnit activeTextureUnit /*= TextureUnit::G0*/) {
    if (handle == 0) return;
    glActiveTexture(GL_TEXTURE0 + static_cast<int>(activeTextureUnit));
    switch (type) {
        case TextureType::TWO_DIMENSIONAL:
            glBindTexture(GL_TEXTURE_2D, handle);
            break;
        case TextureType::CUBEMAP:
            glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
            break;
    }
}

int RenderBackendGL::getShaderModuleType(ShaderModuleType type) {
    switch (type) {
        case ShaderModuleType::VERTEX:
            return GL_VERTEX_SHADER;
        case ShaderModuleType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
    }
    return -1;
}

int RenderBackendGL::createShaderModule(const std::string& shader, ShaderModuleType type) {
    auto data = std::string{GL_VERSION_STRING.data()} + "\n\n" + shader;
    const char* dat = data.c_str();

    int glType = getShaderModuleType(type);
    int handle = glCreateShader(glType);
    glShaderSource(handle, 1, &dat, nullptr);
    glCompileShader(handle);
#if DEBUG
    int success;
    char infoLog[512];
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(handle, 512, nullptr, infoLog);
        LOG_GL.error(TRF("error.shader_resource.compilation_failure", glType, infoLog));
    }
#endif
    return handle;
}

void RenderBackendGL::destroyShaderModule(int handle) {
    glDeleteShader(handle);
}
