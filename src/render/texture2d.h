#pragma once

#include "texture.h"
#include "glad/gl.h"

class texture2d : public texture {
public:
    texture2d(const std::string& provider_, const std::string& name_, int format, int wrapModeU = GL_REPEAT, int wrapModeV = GL_REPEAT, int filterMode = GL_LINEAR, bool mipmaps = true);
    void compile(std::unique_ptr<unsigned char> buffer, unsigned int bufferLen) override;
    void use() override;
protected:
    int format;
    int wrapModeU;
    int wrapModeV;
    int filterMode;
    bool mipmaps;
};
