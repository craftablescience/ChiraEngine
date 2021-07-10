#ifndef BASICGAMEENGINE_TEXTURE_H
#define BASICGAMEENGINE_TEXTURE_H


#include "../utility/image.h"
#include "../utility/glCompilable.h"

class texture : public glCompilable {
public:
    explicit texture(const std::string& file);
    virtual ~texture();
    virtual void compile();
    void discard() override;
    virtual void use() = 0;
    void setTextureUnit(int textureUnit);
    [[nodiscard]] unsigned int getHandle() const;
protected:
    unsigned int handle = 0;
    int activeTextureUnit;
    image* file;
    int width;
    int height;
    int bitDepth;
};


#endif //BASICGAMEENGINE_TEXTURE_H
