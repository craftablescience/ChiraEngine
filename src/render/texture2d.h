#ifndef BASICGAMEENGINE_TEXTURE2D_H
#define BASICGAMEENGINE_TEXTURE2D_H


#include "texture.h"
#include "glad/gl.h"

class texture2d : public texture {
public:
    texture2d(const std::string& file, int format, int wrapModeU = GL_REPEAT, int wrapModeV = GL_REPEAT, int filterMode = GL_LINEAR, bool mipmaps = true);
    void compile() override;
    void use() override;
    void discard() override;
private:
    int format;
    int wrapModeU;
    int wrapModeV;
    int filterMode;
    bool mipmaps;
};


#endif //BASICGAMEENGINE_TEXTURE2D_H
