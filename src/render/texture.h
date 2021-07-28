#pragma once

#include "../loader/image.h"
#include "../utility/compilable.h"

class texture : public compilable {
public:
    explicit texture(const std::string& file);
    virtual ~texture();
    void compile() override;
    void discard() override;
    virtual void use() = 0;
    void setTextureUnit(int textureUnit);
    [[nodiscard]] int getTextureUnit() const;
    [[nodiscard]] unsigned int getHandle() const;
protected:
    unsigned int handle = 0;
    int activeTextureUnit;
    image* file;
    int width;
    int height;
    int bitDepth;
};
