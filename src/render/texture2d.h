#ifndef BASICGAMEENGINE_TEXTURE2D_H
#define BASICGAMEENGINE_TEXTURE2D_H


#include "texture.h"

class texture2d : public texture {
public:
    texture2d(const std::string& file, int format, bool mipmaps);
    void compile() override;
    void use() override;
    void discard() override;
private:
    int format;
    bool mipmaps;
};


#endif //BASICGAMEENGINE_TEXTURE2D_H
