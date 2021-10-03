#pragma once

#include "texture.h"
#include "glad/gl.h"

namespace chira {
    class texture2d : public texture {
    public:
        texture2d(const std::string& identifier_, int format, bool vFlip_ = true, int wrapModeU = GL_REPEAT, int wrapModeV = GL_REPEAT, int filterMode = GL_LINEAR, bool mipmaps = true);
        void compile(unsigned char* buffer, std::size_t bufferLen) override;
        void use() override;
    protected:
        int format;
        int wrapModeU;
        int wrapModeV;
        int filterMode;
        bool mipmaps;
    };
}
